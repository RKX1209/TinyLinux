/*
 * include/asm/irq_vectors.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ASM_IRQ_VECTORS_H
#define _ASM_IRQ_VECTORS_H

#define FIRST_EXTERNAL_VECTOR	0x20
#define SYSCALL_VECTOR		0x80

#define TIMER_IRQ 0
#define NR_VECTORS 256

#define FPU_IRQ	13
#endif
