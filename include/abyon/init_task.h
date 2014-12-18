/*
 * include/abyon/init_task.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ABYON_INIT_TASK_H
#define _ABYON_INIT_TASK_H

#include <abyon/percpu.h>

extern struct mm_struct init_mm;

extern struct task_struct init_task;

extern union thread_union init_thread_union;

#endif
