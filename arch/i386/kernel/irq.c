/*
 * arch/i386/kernel/irq.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/ptrace.h>
extern int printk(const char *fmt, ...);

unsigned int do_IRQ(struct pt_regs *regs){
  printk("do_IRQ");
}
