/*
 * include/asm/hw_irq.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef __ASM_HW_IRQ_H__
#define __ASM_HW_IRQ_H__

#include <asm/irq_vectors.h>
extern void (*interrupt[NR_VECTORS-FIRST_EXTERNAL_VECTOR])(void);

#endif
