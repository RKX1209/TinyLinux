/*
 * arch/i386/kernel/cpu/common.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/init_task.h>
#include <abyon/sched.h>

#include <asm/desc.h>
#include <asm/smp.h>

extern int printk(const char *fmt, ...);

/* [TODO] Init CPU(SMP) info, thread, */

void cpu_init(void){
  int cpu = smp_processor_id();
  struct tss_struct * t = &per_cpu(init_tss,cpu);
  struct thread_struct * thread = &current->thread;
  struct desc_struct *gdt = get_cpu_gdt_table(cpu);
  printk("Initializing CPU#%d",cpu);
  memcpy(gdt,cpu_gdt_table,GDT_SIZE);
  cpu_gdt_descr[cpu].size = GDT_SIZE - 1;
  cpu_gdt_descr[cpu].address = (unsigned long)gdt;

  load_gdt(&cpu_gdt_descr[cpu]);
  load_idt(&idt_descr);
  init_mm.mm_count++;
  current_thread_info()->status = 0;

  /* Delete NT flag */
  __asm__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");


}
