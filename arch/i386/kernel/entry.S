/*
 * arch/i386/kernel/entry.S
 * Copyright (C) <2014>  <@RKX1209>
 */

#define __ASSEMBLY__	
#include <asm/irq_vectors.h>
#include <asm/segment.h>
#include <asm/thread_info.h>

EBX		= 0x00
ECX		= 0x04
EDX		= 0x08
ESI		= 0x0C
EDI		= 0x10
EBP		= 0x14
EAX		= 0x18
DS		= 0x1C
ES		= 0x20
ORIG_EAX	= 0x24
EIP		= 0x28
CS		= 0x2C
EFLAGS		= 0x30
OLDESP		= 0x34
OLDSS		= 0x38

CF_MASK		= 0x00000001
TF_MASK		= 0x00000100
IF_MASK		= 0x00000200
DF_MASK		= 0x00000400 
NT_MASK		= 0x00004000
VM_MASK		= 0x00020000

#define resume_kernel		restore_nocheck /* kernel non preemption */

#define SAVE_ALL \
	cld; \
	pushl %es; \
	pushl %ds; \
	pushl %eax; \
	pushl %ebp; \
	pushl %edi; \
	pushl %esi; \
	pushl %edx; \
	pushl %ecx; \
	pushl %ebx; \
	movl $(__USER_DS), %edx; \
	movl %edx, %ds; \
	movl %edx, %es;
	
#define RESTORE_INT_REGS \
	popl %ebx;	\
	popl %ecx;	\
	popl %edx;	\
	popl %esi;	\
	popl %edi;	\
	popl %ebp;	\
	popl %eax

#define RESTORE_REGS	\
	RESTORE_INT_REGS; \
1:	popl %ds;	\
2:	popl %es;	\
.section .fixup,"ax";	\
3:	movl $0,(%esp);	\
	jmp 1b;		\
4:	movl $0,(%esp);	\
	jmp 2b;		\
.previous;		\
.section __ex_table,"a";\
	.align 4;	\
	.long 1b,3b;	\
	.long 2b,4b;	\
.previous
	
	
ret_from_exception:
ret_from_intr:
	GET_THREAD_INFO(%ebp)
	movl EFLAGS(%esp), %eax
	movb CS(%esp),%al
	testl $(VM_MASK | 3), %eax /* VM mode or user mode */
	jz resume_kernel
resume_userspace:
	cli
	movl 0x8(%ebp), %ecx
	andl $_TIF_WORK_MASK, %ecx
	//jne work_pending
	jmp restore_all
	
/* resume_kernel:
	cli
	cmpl $0, 0x14(%ebp) 
	jnz restore_nocheck */
	
restore_all:
	movl EFLAGS(%esp), %eax
	movb OLDSS(%esp), %ah
	movb CS(%esp), %al
	andl $(VM_MASK | (4 << 8) | 3), %eax
	cmpl $((4 << 8) | 3), %eax
	//je ldt_ss
restore_nocheck:
	RESTORE_REGS
	addl $4, %esp
1:	iret

work_pending:
	nop
	
#define FIXUP_ESPFIX_STACK \
	nop			;
	
#define UNWIND_ESPFIX_STACK \
	pushl %eax		; \
	movl %ss,%eax		;\
	cmpw $__ESPFIX_SS,%ax 	;\
	jne 28f			;\
	movl $__KERNEL_DS, %edx	;\
	movl %edx, %ds		;\
	movl %edx, %es		;\
	FIXUP_ESPFIX_STACK \
28:	popl %eax		;

.global system_call
system_call:
syscall_exit:	
syscall_exit_work:
	jmp resume_userspace
	
.global divide_error
divide_error:	
	pushl $0
	pushl $do_divide_error
	.align 16,0x90
error_code:
	pushl %ds
	pushl %eax
	xorl %eax,%eax
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %edx
	decl %eax
	pushl %ecx
	pushl %ebx
	cld
	pushl %es
	UNWIND_ESPFIX_STACK
	popl %ecx
	movl ES(%esp),%edi /* address of int handler */
	movl ORIG_EAX(%esp), %edx /* Error code */
	movl %eax, ORIG_EAX(%esp)
	movl %ecx,ES(%esp)
	movl $(__USER_DS),%ecx /* Change data segment(user mode) */
	movl %ecx,%ds
	movl %ecx,%es
	movl %esp,%eax
	call *%edi
	jmp ret_from_exception

.global coprocessor_error	
coprocessor_error:
	pushl $0
	pushl $do_coprocessor_error
	jmp error_code
	
.global simd_coprocessor_error
simd_coprocessor_error:
	pushl $0
	pushl $do_simd_coprocessor_error
	jmp error_code
	
.global device_not_available
device_not_available:
	jmp ret_from_exception

.global debug
debug:
	call do_debug
	jmp ret_from_exception

.global nmi
nmi:	
	jmp restore_all
	
.global int3
int3:	
	pushl $-1
	SAVE_ALL
	xorl %edx,%edx
	movl %esp,%eax
	call do_int3
	jmp ret_from_exception

	
.global overflow
overflow:
	pushl $0
	pushl $do_overflow
	jmp error_code

.global bounds
bounds:	
	pushl $0
	pushl $do_bounds
	jmp error_code

.global invalid_op
invalid_op:	
	pushl $0
	pushl $do_invalid_op
	jmp error_code

.global coprocessor_segment_overrun
coprocessor_segment_overrun:	
	pushl $0
	pushl $do_coprocessor_segment_overrun
	jmp error_code

.global invalid_TSS
invalid_TSS:	
	pushl $do_invalid_TSS
	jmp error_code

.global segment_not_present
segment_not_present:	
	pushl $do_segment_not_present
	jmp error_code

.global stack_segment
stack_segment:	
	pushl $do_stack_segment
	jmp error_code

.global general_protection
general_protection:	
	pushl $do_general_protection
	jmp error_code
	.previous .text

.global alignment_check
alignment_check:	
	pushl $do_alignment_check
	jmp error_code

.global page_fault
page_fault:	
	pushl $do_page_fault
	jmp error_code

.global spurious_interrupt_bug
spurious_interrupt_bug:	
	pushl $0
	pushl $do_spurious_interrupt_bug
	jmp error_code
