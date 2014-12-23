/*
 * kernel/main.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/sched.h>
#include <asm/system.h>
#include <drivers/disp.h>

extern void io_hlt(void);

extern int printk(const char *fmt, ...);
extern void setup_arch();
extern void trap_init();
extern void init_IRQ();
extern void init_timers();
extern void softirq_init();
extern void key_int();
extern void kernel_main(void);


void kernel_main(void){
  printk("Welcome to Abyon kernel!");
  setup_arch();
  
  sched_init();

  trap_init();

  init_IRQ();

  init_timers();

  softirq_init();

  local_irq_enable();
  
  key_int();


  printk("Enter sleeping...");
  for(;;){
    io_hlt();
  }
  return;
}
