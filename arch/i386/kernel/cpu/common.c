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
  printk("Initializing CPU#%d",cpu);
  struct tss_struct * t = &per_cpu(init_tss,cpu);
  struct thread_struct * thread = &current->thread;
  struct desc_struct *gdt = get_cpu_gdt_table(cpu);
  //memcpy(gdt,cpu_gdt_table,GDT_SIZE);
  //cpu_gdt_descr[cpu].size = GDT_SIZE - 1;
  //cpu_gdt_descr[cpu].address = (unsigned long)gdt;

  //printk("GDTbef(desc) = 0x%x",ga);

  //unsigned long *gp = (unsigned long)(&cpu_gdt_descr[cpu]);
  //unsigned long *ip = (unsigned long)(&idt_descr);

  //printk("gdt_descr[0](0x%x)",&cpu_gdt_descr[cpu]);
  //printk("idt_descr(0x%x)",&idt_descr);
  
  //load_gdt(&cpu_gdt_descr[cpu]);
  /* __asm__ __volatile__("movl $0x60,%edx ; movl %edx,%cs"); */
  /* __asm__ __volatile__("movl $0x68,%edx ; movl %edx,%ds"); */
  load_idt(&idt_descr);
  init_mm.mm_count++;
  current_thread_info()->status = 0;

  /* Delete NT flag */
  __asm__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");

  unsigned long long ia,ga;
  //load_esp0(t,thread);
  set_tss_desc(cpu,t);
  load_TR_desc();

  store_idt(&ia);
  store_gdt(&ga);
  printk("GDT(desc) = 0x%x",ga);
  printk("IDT(desc) = 0x%x",ia);
  //printk("ia(0x%x)",&ia);
  //printk("esp0 = 0x%x",t->esp0);
  __asm__ __volatile__("xorl %eax, %eax; movl %eax, %fs; movl %eax, %gs");
}
