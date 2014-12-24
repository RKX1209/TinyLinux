/*
 * arch/i386/kernel/traps.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/kernel.h>
#include <abyon/ptrace.h>
#include <abyon/sched.h>

#include <asm/irq_vectors.h>
#include <asm/processor.h>
#include <asm/segment.h>

extern int printk(const char *fmt, ...);
extern void cpu_init(void);
struct desc_struct idt_table[256] __attribute__((__section__(".data.idt"))) = { {0,0},};

/* IDT functions*/
int system_call(void);
void divide_error(void);
void debug(void);
void nmi(void);
void int3(void);
void overflow(void);
void bounds(void);
void invalid_op(void);
void device_not_available(void);
void coprocessor_segment_overrun(void);
void invalid_TSS(void);
void segment_not_present(void);
void stack_segment(void);
void general_protection(void);
void page_fault(void);
void coprocessor_error(void);
void simd_coprocessor_error(void);
void alignment_check(void);
void spurious_interrupt_bug(void);
void machine_check(void);

#define DO_ERROR(trapnr, signr, str, name) \
fastcall void do_##name() \
{ \
 return; \
}

#define DO_ERROR_INFO(trapnr, signr, str, name, sicode, siaddr) \
fastcall void do_##name() \
{ \
 printk("do_error_info"); \
 return; \
}

#define DO_VM86_ERROR(trapnr, signr, str, name) \
fastcall void do_##name() \
{ \
 return; \
}

#define DO_VM86_ERROR_INFO(trapnr, signr, str, name, sicode, siaddr) \
fastcall void do_##name() \
{ \
 return; \
}

/* Define trap gate handlers */
DO_VM86_ERROR_INFO( 0, SIGFPE,  "divide error", divide_error, FPE_INTDIV, regs->eip)
DO_VM86_ERROR( 3, SIGTRAP, "int3", int3)
DO_VM86_ERROR( 4, SIGSEGV, "overflow", overflow)
DO_VM86_ERROR( 5, SIGSEGV, "bounds", bounds)
DO_ERROR_INFO( 6, SIGILL,  "invalid operand", invalid_op, ILL_ILLOPN, regs->eip)
DO_ERROR( 9, SIGFPE,  "coprocessor segment overrun", coprocessor_segment_overrun)
DO_ERROR(10, SIGSEGV, "invalid TSS", invalid_TSS)
DO_ERROR(11, SIGBUS,  "segment not present", segment_not_present)
DO_ERROR(12, SIGBUS,  "stack segment", stack_segment)
DO_ERROR_INFO(17, SIGBUS, "alignment check", alignment_check, BUS_ADRALN, 0)
DO_ERROR_INFO(32, SIGSEGV, "iret exception", iret_error, ILL_BADSTK, 0)

fastcall void do_general_protection(struct pt_regs * regs, long error_code){
}
fastcall void do_nmi(struct pt_regs * regs, long error_code){
}
fastcall void do_debug(struct pt_regs * regs, long error_code){
}
fastcall void do_coprocessor_error(struct pt_regs * regs, long error_code){
}
fastcall void do_simd_coprocessor_error(struct pt_regs * regs, long error_code){
}
fastcall void do_spurious_interrupt_bug(struct pt_regs * regs, long error_code){
}
fastcall void do_page_fault(struct pt_regs * regs, long error_code){
}

#define _set_gate(gate_addr,type,dpl,addr,seg) \
do { \
  int __d0, __d1; \
  __asm__ __volatile__ ("movw %%dx,%%ax\n\t" \
	"movw %4,%%dx\n\t" \
	"movl %%eax,%0\n\t" \
	"movl %%edx,%1" \
	:"=m" (*((long *) (gate_addr))), \
	 "=m" (*(1+(long *) (gate_addr))), "=&a" (__d0), "=&d" (__d1) \
	:"i" ((short) (0x8000+(dpl<<13)+(type<<8))), \
	 "3" ((char *) (addr)),"2" ((seg) << 16)); \
} while (0)

void set_intr_gate(unsigned int n, void *addr){
  _set_gate(idt_table+n,14,0,addr,__KERNEL_CS);
}

static inline void set_system_intr_gate(unsigned int n, void *addr){
  _set_gate(idt_table+n, 14, 3, addr, __KERNEL_CS);
}

static void set_trap_gate(unsigned int n, void *addr){
  _set_gate(idt_table+n,15,0,addr,__KERNEL_CS);
}

static void set_system_gate(unsigned int n, void *addr){
  _set_gate(idt_table+n,15,3,addr,__KERNEL_CS);
}

static void set_task_gate(unsigned int n, unsigned int gdt_entry){
  _set_gate(idt_table+n,5,0,0,(gdt_entry<<3));
}

void trap_init(void){
  set_trap_gate(0,&divide_error);
  set_intr_gate(1,&debug);
  set_intr_gate(2,&nmi);
  set_system_intr_gate(3, &int3); /* int3-5 can be called from all */
  set_system_gate(4,&overflow);
  set_system_gate(5,&bounds);
  set_trap_gate(6,&invalid_op);
  set_trap_gate(7,&device_not_available);
  //set_task_gate(8,GDT_ENTRY_DOUBLEFAULT_TSS);
  set_trap_gate(9,&coprocessor_segment_overrun);
  set_trap_gate(10,&invalid_TSS);
  set_trap_gate(11,&segment_not_present);
  set_trap_gate(12,&stack_segment);
  set_trap_gate(13,&general_protection);
  set_intr_gate(14,&page_fault);
  set_trap_gate(15,&spurious_interrupt_bug);
  set_trap_gate(16,&coprocessor_error);
  set_trap_gate(17,&alignment_check);
  set_trap_gate(19,&simd_coprocessor_error);

  set_system_gate(SYSCALL_VECTOR,&system_call);
  
  /* Set GDT,TSS,IDT...etc */
  cpu_init();

  printk("trap_init... [OK]");  
  return;
}
