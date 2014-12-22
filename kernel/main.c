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
extern void kernel_main(void);
extern int _text_start;
extern int _text_end;
extern int _data_start;
extern int _data_end;
extern int _bss_start;
extern int _bss_end;

void kernel_main(void){
  int i;
  char *p;
  printk("Welcome to Abyon kernel!");
  setup_arch();
  
  sched_init();

  trap_init();

  init_IRQ();

  init_timers();
  local_irq_enable();

  
  key_int();
  /* printk("text:[0x%x,0x%x]",&(_text_start),&(_text_end)); */
  /* printk("data:[0x%x,0x%x]",&(_data_start),&(_data_end)); */
  /* printk("bss:[0x%x,0x%x]",&(_bss_start),&(_bss_end)); */
  /* printk("pagetable:[0x%x]",swapper_pg_dir); */
  /* printk("font:[0x%x]",&kernel_end); */

  printk("Enter sleeping...");
  for(;;){
    io_hlt();
  }
  return;
}
