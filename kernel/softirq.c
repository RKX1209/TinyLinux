/*
 * kernel/softirq.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/interrupt.h>
#include <abyon/percpu.h>

#include <asm/smp.h>

extern int printk(const char *fmt, ...);

static struct softirq_action softirq_vec[32];

struct tasklet_head{
  struct tasklet_struct *list;
};

static DEFINE_PER_CPU(struct task_struct *,ksoftirqd);
static DEFINE_PER_CPU(struct tasklet_head, tasklet_vec) = { 0 };
static DEFINE_PER_CPU(struct tasklet_head, tasklet_hi_vec) = { 0 };

static void tasklet_action(struct softirq_action *a){
struct tasklet_struct *list = per_cpu(tasklet_vec,smp_processor_id()).list;
per_cpu(tasklet_vec,smp_processor_id()).list = 0;
while(list){
struct tasklet_struct *t = list;
list = list->next;
t->func(t->data);
}
}

static void tasklet_hi_action(struct softirq_action *a){
struct tasklet_struct *list = per_cpu(tasklet_hi_vec,smp_processor_id()).list;
per_cpu(tasklet_hi_vec,smp_processor_id()).list = 0;
while(list){
struct tasklet_struct *t = list;
list = list->next;
t->func(t->data);
}
}

void softirq_init(void){
  open_softirq(TASKLET_SOFTIRQ,tasklet_action,0);
  open_softirq(HI_SOFTIRQ,tasklet_hi_action,0);
printk("softirq_init... [OK]");
}

void open_softirq(int nr,void (*action)(struct softirq_action*),void *data){
  softirq_vec[nr].data = data;
  softirq_vec[nr].action = action;
}
