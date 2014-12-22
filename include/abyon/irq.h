/*
 * include/abyon/irq.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef _ABYON_IRQ_H
#define _ABYON_IRQ_H

#include <abyon/kernel.h>
#include <abyon/interrupt.h>

#include <asm/irq.h>

#define IRQ_INPROGRESS	1	/* IRQ handler active - do not enter! */
#define IRQ_DISABLED	2	/* IRQ disabled - do not enter! */
#define IRQ_PENDING	4	/* IRQ pending - replay on enable */
#define IRQ_REPLAY	8	/* IRQ has been replayed but not acked yet */
#define IRQ_AUTODETECT	16	/* IRQ is being autodetected */
#define IRQ_WAITING	32	/* IRQ not yet seen - for autodetection */
#define IRQ_LEVEL	64	/* IRQ level triggered */
#define IRQ_MASKED	128	/* IRQ masked - shouldn't be seen again */

struct hw_interrupt_type{
  const char *typename;
  unsigned int (*startup)(unsigned int irq);
  void (*shutdown)(unsigned int irq);
  void (*enable)(unsigned int irq);
  void (*disable)(unsigned int irq);
  void (*ack)(unsigned int irq);
  void (*end)(unsigned int irq);
  void (*set_affinity)(unsigned int irq,unsigned long cpumask[10]);

};
typedef struct irq_desc{
  struct hw_interrupt_type *handler;
  void *handler_data;
struct irqaction *action;
  unsigned long status;
  unsigned long depth;
  unsigned long irq_count;
  unsigned long irqs_unhandled;
  
}irq_desc_t;
struct hw_interrupt_type no_irq_type;
irq_desc_t irq_desc [NR_IRQS];

extern fastcall int handle_IRQ_event(unsigned int irq, struct pt_regs *regs,
					struct irqaction *action);
extern fastcall unsigned int __do_IRQ(unsigned int irq, struct pt_regs *regs);
#endif
