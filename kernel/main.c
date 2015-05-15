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
  setup_arch();//init e820, bootmem, pagetable(pgd,pte...), fix area  paging on
  
  sched_init(); //init runqueue, current <= idle_proc

  trap_init(); //init trap tables

  init_IRQ(); //init 8259A, IQR table[i] = interrupt[i] => do_IRQ(entry.S), init PIT

  init_timers();//init softirq[TIMER_SOFTIRQ] => timer

  softirq_init();//init softirq[TASKLET,HI_SOFTIRQ]

  local_irq_enable();
  
  key_int();

  vfs_caches_init_early();//init dentry cache, init inode cache

  //mem_init(); //free bootmem area
  //kmem_cache_init(); //init slab allocator
  

  printk("Enter sleeping...");
  for(;;){
    io_hlt();
  }
  return;
}
