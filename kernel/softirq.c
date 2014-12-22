/*
 * kernel/softirq.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/interrupt.h>
#include <abyon/percpu.h>

static struct softirq_action softirq_vec[32];

static DEFINE_PER_CPU(struct task_struct *,ksoftirqd);

void open_softirq(int nr,void (*action)(struct softirq_action*),void *data){
  softirq_vec[nr].data = data;
  softirq_vec[nr].action = action;
}
