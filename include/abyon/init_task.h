/*
 * include/abyon/init_task.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ABYON_INIT_TASK_H
#define _ABYON_INIT_TASK_H

#include <abyon/percpu.h>
#include <asm/processor.h>
#include <asm/segment.h>

#define init_stack		(init_thread_union.stack)

#define INIT_TSS  {							\
	.esp0		= sizeof(init_stack) + (long)&init_stack,	\
	.ss0		= __KERNEL_DS,					\
	.ss1		= __KERNEL_CS,					\
	.io_bitmap_base	= INVALID_IO_BITMAP_OFFSET,			\
	.io_bitmap	= { [ 0 ... IO_BITMAP_LONGS] = ~0 },		\
}

extern struct mm_struct init_mm;

extern struct task_struct init_task;

extern union thread_union init_thread_union;

DECLARE_PER_CPU(struct tss_struct, init_tss);

#endif
