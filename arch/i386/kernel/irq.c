/*
 * arch/i386/kernel/irq.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/irq.h>
#include <abyon/ptrace.h>

extern int printk(const char *fmt, ...);

fastcall unsigned int do_IRQ(struct pt_regs *regs){
  int irq = regs->orig_eax & 0xff;   
  __do_IRQ(irq,regs);
  return 1;
}

fastcall unsigned int __do_IRQ(unsigned int irq,struct pt_regs *regs){
  printk("__do_IRQ 0x%x",irq);
  irq_desc_t *desc = irq_desc + irq;
  
  if(desc->handler->ack) desc->handler->ack(irq);
  unsigned long status = desc->status & ~(IRQ_REPLAY | IRQ_WAITING);
  status |= IRQ_PENDING;

  struct irqaction *action = 0;
  if(!(status & (IRQ_DISABLED | IRQ_INPROGRESS))){
    action = desc->action;
    status &= ~IRQ_PENDING;
    status |= IRQ_INPROGRESS;
  }
  desc->status = status; 
int action_ret = handle_IRQ_event(irq,regs,action);
  desc->status &= ~IRQ_INPROGRESS;
  desc->handler->end(irq);
  return 1;
}

fastcall int handle_IRQ_event(unsigned int irq,struct pt_regs *regs,struct irqaction *action){
  int retval = 0;
//if(!(action->flags & SA_INTERRUPT)) local_irq_enable();
while(action){
    int ret = action->handler(irq,action->dev_id,regs);
    retval |~ ret;
    action = action->next;
}
  return retval;
}
