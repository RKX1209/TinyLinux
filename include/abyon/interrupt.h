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

enum
{
  HI_SOFTIRQ=0,
  TIMER_SOFTIRQ,
  NET_TX_SOFTIRQ,
  NET_RX_SOFTIRQ,
  SCSI_SOFTIRQ,
  TASKLET_SOFTIRQ
};

struct softirq_action{
  void (*action)(struct softirq_action *);
  void *data;
};

extern void open_softirq(int nr, void (*action)(struct softirq_action*), void *data);
extern void softirq_init(void);

enum
{
  TASKLET_STATE_SCHED,	/* Tasklet is scheduled for execution */
  TASKLET_STATE_RUN	/* Tasklet is running (SMP only) */
};

struct tasklet_struct{
  struct tasklet_struct *next;
  unsigned long state;
  void (*func)(unsigned long);
  unsigned long data;
};

#define DECLARE_TASKLET(name, func, data) \
struct tasklet_struct name = { 0, 0, 0, func, data }

#define DECLARE_TASKLET_DISABLED(name, func, data) \
struct tasklet_struct name = { 0, 0, 1, func, data }

#endif
