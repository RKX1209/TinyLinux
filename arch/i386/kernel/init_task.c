/*
 * arch/i386/kernel/init_task.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/init_task.h>
#include <abyon/sched.h>

#include <asm/desc.h>
#include <asm/processor.h>
#include <asm/thread_info.h>

/* struct mm_struct init_mm = INIT_MM(init_mm); */

/* struct task_struct init_task = INIT_TASK(init_task); */

struct mm_struct init_mm;

struct task_struct init_task;

union thread_union init_thread_union __attribute__((__section__(".data.init_task")));

DEFINE_PER_CPU(struct tss_struct, init_tss);
