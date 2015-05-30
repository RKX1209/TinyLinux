/*
 * kernel/sched.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/init_task.h>
#include <abyon/list.h>
#include <abyon/percpu.h>
#include <abyon/sched.h>
#include <abyon/threads.h>
#include <abyon/thread_info.h>

#include <asm/bitops.h>
#include <asm/smp.h>
#include <asm/system.h>

extern int printk(const char *fmt, ...);

static DEFINE_PER_CPU(struct runqueue,runqueues);

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define CURRENT_BONUS(p) 1

void sched_init(void){
  printk("sched_init...");
  runqueue_t *rq;
  int i,j,k;
  for(i = 0; i < NR_CRUS; i++){
    rq = cpu_rq(i);
    rq->active = rq->arrays;
    rq->expired = rq->arrays + 1;
    rq->best_expired_prio = MAX_PRIO;
  }
  for(j = 0; j < 2; j++){
    prio_array_t *array = rq->arrays + j;
    for(k = 0; k < MAX_PRIO; k++){
      INIT_LIST_HEAD(array->queue + k);
      __clear_bit(k,array->bitmap);
    }
     __set_bit(MAX_PRIO,array->bitmap);
  }
  init_mm.mm_count++;
  init_idle(current,smp_processor_id());
  printk("sched_init... [OK]");
}

void init_idle(task_t *idle, int cpu){
  runqueue_t *rq = cpu_rq(cpu);
  idle->array = 0;
  idle->prio = MAX_PRIO;
  idle->state = TASK_RUNNING;
  set_task_cpu(idle,cpu);
  rq->curr = rq->idle = idle;
}
static void dequeue_task(struct task_struct *p, prio_array_t *array) {
  list_del(&p->run_list);
  if(list_empty(array->queue + p->prio))
    __clear_bit(p->prio,array->bitmap);
  
}

static void deactivate_task(struct task_struct *p, runqueue_t *rq) {
  rq->nr_runnning--;
  dequeue_task(p,p->array);
  p->array = 0;
}

static int effective_prio(task_t *p){
  return p->prio;  
}

static void recalc_task_prio(task_t *p,unsigned long long now){
  unsigned long long sleep_time = now - p->timestamp;
  if(sleep_time > 0){    
  }
  p->prio = effective_prio(p);
}

static void enqueue_task(struct task_struct *p, prio_array_t *array) {
  list_add_tail(&p->run_list,array->queue + p->prio);
  __set_bit(p->prio,array->bitmap);
  array->nr_active++;
  p->array = array;
}

static inline void switch_mm(struct mm_struct *prev,
			     struct mm_struct *next,
			     struct task_struct *tsk){
  int cpu = smp_processor_id();
  write_cr3(next->pgd);
  
}

static inline task_t *context_switch(runqueue_t *rq,task_t *prev,task_t *next){
  struct mm_struct *mm = next->mm;
  struct mm_struct *oldmm = prev->active_mm;
  switch_mm(oldmm,mm,next);
  switch_to(prev,next,prev);
  return prev;
}

void scheduler_tick(int user_ticks,int sys_ticks){
  int cpu = smp_processor_id();
  runqueue_t *rq = cpu_rq(cpu);
  //rq->timestamp_last_tick = sched_clock();
  task_t *p = current;
  if(!--p->time_slice){
    dequeue_task(p,rq->active);
    set_ti_thread_flag(p->thread_info,TIF_NEED_RESCHED);
    p->prio = effective_prio(p);
    enqueue_task(p,rq->expired);    
  }else{    
  }
  
}

void schedule(){
  task_t *prev,*next;
 need_resched:
  prev = current;
  runqueue_t *rq = cpu_rq(smp_processor_id());
  unsigned long long now = sched_clock();
  unsigned long long run_time;
  run_time = now - prev->timestamp;
  run_time /= CURRENT_BONUS(prev);
  if(prev->state){
    if(prev->state & TASK_INTERRUPTIBLE)
      prev->state = TASK_RUNNING;
    else{
      deactivate_task(prev,rq);
    }
  }
  if(!(rq->nr_runnning)){
  go_idle:
    ;
    //idle_balance(smp_processor_id(),rq);    
  }else{
    next = rq->idle;
    goto switch_tasks;
  }
  prio_array_t *array = rq->active;
  if(!array->nr_active){
    rq->active = rq->expired;
    rq->expired = array;
    array = rq->active;    
  }
  int idx = sched_find_first_bit(array->bitmap);
  struct list_head *queue = array->queue + idx;
  //next = list_entry(queue->next,task_t,run_list);
  if(next->activated > 0){
    unsigned long long delta = now - next->timestamp;
    array = next->array;
    dequeue_task(next,array);
    recalc_task_prio(next,next->timestamp + delta);
    enqueue_task(next,array);
  }
 switch_tasks:
  if(prev != next){
    next->timestamp = now;
    rq->nr_switches++;
    rq->curr = next;
    prev = context_switch(rq,prev,next);
  }
}
