/*
 * include/abyon/interrupt.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef _ABYON_INTERRUPT_H
#define _ABYON_INTERRUPT_H

#include <abyon/ptrace.h>

#define IRQ_NONE	(0)
#define IRQ_HANDLED	(1)

#define SA_RESTART	0x10000000u
#define SA_SAMPLE_RANDOM SA_RESTART
#define SA_INTERRUPT	0x20000000 
#define SA_SHIRQ	0x04000000

struct irqaction{
  int (*handler)(int,void *,struct pt_regs *);
  unsigned long flags;
  const char *name;
  void *dev_id;
  struct irqaction *next;
  int irq;
};


#endif
