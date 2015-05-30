/*
 * arch/i386/kernel/process.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/sched.h>

#include <asm/bitops.h>
#include <asm/ptrace.h>
#include <asm/segment.h>
#include <asm/smp.h>
#include <asm/system.h>

extern long do_fork(unsigned long,unsigned long,struct pt_regs *,
		      unsigned long,int *,int *);

extern void kernel_thread_helper(void);
__asm__(".section .text\n"
	".align 4\n"
	"kernel_thread_helper:\n\t"
	"movl %edx,%eax\n\t"
	"pushl %edx\n\t"
	"call *%ebx\n\t"
	"pushl %eax\n\t"
	"call do_exit\n"
	".previous");

int kernel_thread(int (*fn)(void*),void *arg,unsigned long flags){
  struct pt_regs regs;
  memset(&regs,0,sizeof(regs));
  regs.ebx = (unsigned long)fn;
  regs.edx = (unsigned long)arg;

  regs.xds = __USER_DS;
  regs.xes = __USER_DS;
  regs.xes = __KERNEL_CS;
  regs.eip = (unsigned long)kernel_thread_helper;
  return do_fork(flags,0,&regs,0,0,0);
  
}

struct task_struct __attribute__((regparm(3))) * __switch_to(struct task_struct *prev_p,struct task_struct *next_p){
  struct thread_struct *prev = &prev_p->thread,*next = &next_p->thread;
  int cpu = smp_processor_id();
  struct tss_struct *tss = &per_cpu(init_tss,cpu);
  load_esp0(tss,next);
  /* __asm__ __volatile__("movl %%fs,%0":"=m" (*(int *)&prev->fs)); */
  /* __asm__ __volatile__("movl %%gs,%0":"=m" (*(int *)&prev->gs)); */


}

void cpu_idle(void){
  while(1){
    schedule();
  }
}

int sys_execve(struct pt_regs regs){
  char *filename = getname((char *)regs.ebx);
  int error = do_execve(filename,
			(char *)regs.ecx,
			(char *)regs.edx,
			&regs);
  return error;
}
