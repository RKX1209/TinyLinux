/*
 * kernel/sched.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/init_task.h>
#include <abyon/list.h>
#include <abyon/percpu.h>
#include <abyon/sched.h>
#include <abyon/threads.h>

#include <asm/bitops.h>
#include <asm/smp.h>

extern int printk(const char *fmt, ...);

static DEFINE_PER_CPU(struct runqueue,runqueues);

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))

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
