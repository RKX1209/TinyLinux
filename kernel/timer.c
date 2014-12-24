/*
 * kernel/timer.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/kernel.h>
#include <abyon/interrupt.h>
#include <abyon/jiffies.h>
#include <abyon/list.h>
#include <abyon/notifier.h>
#include <abyon/percpu.h>
#include <abyon/timer.h>

#include <asm/smp.h>

#define TVN_BITS 4
#define TVR_BITS 6
#define TVN_SIZE (1 << TVN_BITS)
#define TVR_SIZE (1 << TVR_BITS)
#define TVN_MASK (TVN_SIZE - 1)
#define TVR_MASK (TVR_SIZE - 1)

extern int printk(const char *fmt, ...);

unsigned long long jiffies = INIT_JIFFIES;
typedef struct tvec_s{
  struct list_head vec[TVN_SIZE];
}tvec_t;

typedef struct tvec_root_s{
  struct list_head vec[TVR_SIZE];
}tvec_root_t;

typedef struct tvec_t_base_s{
  unsigned long timer_jiffies;
  struct timer_list *running_timer;
  tvec_root_t tv1;
  tvec_t tv2;
  tvec_t tv3;
  tvec_t tv4;
  tvec_t tv5;  
}tvec_base_t;

static DEFINE_PER_CPU(tvec_base_t, tvec_bases);

static inline void detach_timer(struct timer_list *timer,
				int clear_pending){
  struct list_head *entry = &timer->entry;
  __list_del(entry->prev, entry->next);
  if (clear_pending)
    entry->next = 0;
}

static void internal_add_timer(tvec_base_t *base, struct timer_list *timer){
  unsigned long expires = timer->expires;
  unsigned long idx = expires - base->timer_jiffies;
  struct list_head *vec;

  if (idx < TVR_SIZE) {
    int i = expires & TVR_MASK;
    vec = base->tv1.vec + i;
  } else if (idx < 1 << (TVR_BITS + TVN_BITS)) {
    int i = (expires >> TVR_BITS) & TVN_MASK;
    vec = base->tv2.vec + i;
  } else if (idx < 1 << (TVR_BITS + 2 * TVN_BITS)) {
    int i = (expires >> (TVR_BITS + TVN_BITS)) & TVN_MASK;
    vec = base->tv3.vec + i;
  } else if (idx < 1 << (TVR_BITS + 3 * TVN_BITS)) {
    int i = (expires >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK;
    vec = base->tv4.vec + i;
  } else if ((signed long) idx < 0) {
    vec = base->tv1.vec + (base->timer_jiffies & TVR_MASK);
  } else {
    int i;
    if (idx > 0xffffffffUL) {
      idx = 0xffffffffUL;
      expires = idx + base->timer_jiffies;
    }
    i = (expires >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK;
    vec = base->tv5.vec + i;
  }
  list_add_tail(&timer->entry, vec);
}

static void init_timer_cpu(int cpu){
  tvec_base_t *base;
  base = &per_cpu(tvec_bases,cpu);
  int j;
  for(j = 0; j < TVN_SIZE; j++){
    INIT_LIST_HEAD(base->tv2.vec + j);
    INIT_LIST_HEAD(base->tv3.vec + j);
    INIT_LIST_HEAD(base->tv4.vec + j);
    INIT_LIST_HEAD(base->tv5.vec + j);
  }
  for(j = 0; j < TVR_SIZE; j++)
    INIT_LIST_HEAD(base->tv1.vec + j);
  base->timer_jiffies = jiffies;
}

static int cascade(tvec_base_t *base,tvec_t *tv,int index){
  struct list_head *head = tv->vec + index;
  struct list_head *curr = head->next;
  while(curr != head){
    struct timer_list *tmp = list_entry(curr,struct timer_list,entry);
    curr = curr->next;
    internal_add_timer(base,tmp);
    
  }
  INIT_LIST_HEAD(head);
  return index;  
}

#define INDEX(N) (base->timer_jiffies >> (TVR_BITS + N * TVN_BITS)) & TVN_MASK

static void __run_timers(tvec_base_t *base){
  while(jiffies >= (base->timer_jiffies)){
    struct list_head work_list = LIST_HEAD_INIT(work_list);
    struct list_head *head = &work_list;
    int index = base->timer_jiffies & TVR_MASK;
    if (!index &&
	(!cascade(base, &base->tv2, INDEX(0))) &&
	(!cascade(base, &base->tv3, INDEX(1))) &&
	!cascade(base, &base->tv4, INDEX(2)))
      cascade(base, &base->tv5, INDEX(3));
    ++(base->timer_jiffies);
    list_splice_init(base->tv1.vec + index,&work_list);
    while(!list_empty(head)){
      void (*fn)(unsigned long);
      unsigned long data;
      struct timer_list *timer = list_entry(head->next,struct timer_list,entry);
      fn = timer->function;
      data = timer->data;
      detach_timer(timer,1);
      fn(data);
    }
  }
}
static int timer_cpu_notify(unsigned long action,void *hcpu){
  long cpu = (long)hcpu;
  switch(action){
  case CPU_UP_PREPARE:
    init_timer_cpu(cpu);
    break;
  default:
    break;
  }
  return NOTIFY_OK;
}

static void run_timer_softirq(struct softirq_action *h){
  tvec_base_t *base = &per_cpu(tvec_bases,smp_processor_id());
  if(jiffies >= (base->timer_jiffies)) __run_timers(base);
}

void init_timers(void){
  timer_cpu_notify((unsigned long)CPU_UP_PREPARE,(void *)(long)smp_processor_id());
  open_softirq(TIMER_SOFTIRQ, run_timer_softirq, 0);
  printk("init_timers... [OK]");
}

