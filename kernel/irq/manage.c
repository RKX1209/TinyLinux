/*
 * kernel/irq/manage.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/errno.h>
#include <abyon/irq.h>

int setup_irq(unsigned int irq, struct irqaction *new){
  struct irq_desc *desc = irq_desc + irq;
  struct irqaction *old,**p;
  unsigned long flags;
  int shared = 0;
  if(desc->handler == &no_irq_type) return -ENOSYS;
  p = &desc->action;
  if((old = *p) != 0){
    if(!(old->flags & new->flags & SA_SHIRQ)){
      return -EBUSY;
    }
    do{
      p = &old->next;
      old = *p;
    }while(old);
    shared = 1;
  }
  *p = new;
  if (!shared) {
    desc->depth = 0;
    desc->status &= ~(IRQ_DISABLED | IRQ_AUTODETECT |
		      IRQ_WAITING | IRQ_INPROGRESS);
    if (desc->handler->startup)
      desc->handler->startup(irq);
    else
      desc->handler->enable(irq);
  }
  new->irq = irq;
  return 0;
}
