/*
 * include/asm/thread_info.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ASM_THREAD_INFO_H
#define _ASM_THREAD_INFO_H


#define THREAD_SIZE 8192

#ifndef __ASSEMBLY__

struct thread_info{
  struct task_struct *task;
  struct exec_domain *exec_domain;
  unsigned long flags;
  unsigned long status;
  unsigned long cpu;
  int preempt_count;
  unsigned long addr_limit;
  
};

union thread_union{
  struct thread_info thread_info;
  unsigned long stack[THREAD_SIZE / sizeof(long)];
};


#define init_thread_info	(init_thread_union.thread_info)
#define init_stack		(init_thread_union.stack)

#define alloc_thread_info(tsk) kmalloc(THREAD_SIZE,0)
#define free_thread_info(info) kfree(info)

static inline struct thread_info *current_thread_info(void){
  struct thread_info *ti;
  __asm__("andl %%esp,%0; ":"=r"(ti) : "0" ((~(THREAD_SIZE - 1))));
  return ti;
}
#else

#define GET_THREAD_INFO(reg) \
  movl $-THREAD_SIZE,reg; \
  andl %esp,reg

#endif

#define TIF_SYSCALL_TRACE	0	/* syscall trace active */
#define TIF_NOTIFY_RESUME	1	/* resumption notification requested */
#define TIF_SIGPENDING		2	/* signal pending */
#define TIF_NEED_RESCHED	3	/* rescheduling necessary */
#define TIF_SINGLESTEP		4	/* restore singlestep on return to user mode */
#define TIF_IRET		5	/* return with iret */
#define TIF_SYSCALL_EMU		6	/* syscall emulation active */
#define TIF_SYSCALL_AUDIT	7	/* syscall auditing active */
#define TIF_SECCOMP		8	/* secure computing */
#define TIF_POLLING_NRFLAG	16	/* true if poll_idle() is polling TIF_NEED_RESCHED */
#define TIF_MEMDIE		17

#define _TIF_SYSCALL_TRACE	(1<<TIF_SYSCALL_TRACE)
#define _TIF_NOTIFY_RESUME	(1<<TIF_NOTIFY_RESUME)
#define _TIF_SIGPENDING		(1<<TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1<<TIF_NEED_RESCHED)
#define _TIF_SINGLESTEP		(1<<TIF_SINGLESTEP)
#define _TIF_IRET		(1<<TIF_IRET)
#define _TIF_SYSCALL_EMU	(1<<TIF_SYSCALL_EMU)
#define _TIF_SYSCALL_AUDIT	(1<<TIF_SYSCALL_AUDIT)
#define _TIF_SECCOMP		(1<<TIF_SECCOMP)
#define _TIF_POLLING_NRFLAG	(1<<TIF_POLLING_NRFLAG)

/* work to do on interrupt/exception return */
#define _TIF_WORK_MASK \
  (0x0000FFFF & ~(_TIF_SYSCALL_TRACE|_TIF_SYSCALL_AUDIT|_TIF_SINGLESTEP|\
		  _TIF_SECCOMP|_TIF_SYSCALL_EMU))

#define INIT_THREAD_INFO(tsk)			\
{						\
	.task		= &tsk,			\
	.exec_domain	= 0,	\
	.flags		= 0,			\
	.cpu		= 0,			\
	.preempt_count	= 1,			\
	.addr_limit	= __KERNEL_DS,		\
}
#endif


