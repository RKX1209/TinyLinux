/*
 * include/asm/desc.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef __ASM_DESC_H
#define __ASM_DESC_H

#include <abyon/kernel.h>
#include <abyon/percpu.h>

#include <asm/processor.h>
#include <asm/segment.h>
#include <asm/smp.h>

extern struct desc_struct cpu_gdt_table[GDT_ENTRIES];
DECLARE_PER_CPU(struct desc_struct, cpu_gdt_table[GDT_ENTRIES]);

#define get_cpu_gdt_table(_cpu) (cpu_gdt_table)

struct Xgt_desc_struct{
  unsigned short size;
  unsigned long address __attribute__((packed));
  unsigned short pad;
}__attribute__((packed));

extern struct Xgt_desc_struct idt_descr, cpu_gdt_descr[1];

#define load_TR_desc() __asm__ __volatile__("ltr %w0"::"q" (GDT_ENTRY_TSS*8))
#define load_LDT_desc() __asm__ __volatile__("lldt %w0"::"q" (GDT_ENTRY_LDT*8))

#define load_gdt(dtr) __asm__ __volatile__("lgdt %0"::"m" (*dtr))
#define load_idt(dtr) __asm__ __volatile__("lidt %0"::"m" (*dtr))
#define load_tr(tr) __asm__ __volatile__("ltr %0"::"mr" (tr))
#define load_ldt(ldt) __asm__ __volatile__("lldt %0"::"mr" (ldt))

#define store_gdt(dtr) __asm__ __volatile__("sgdt %0":"=m" (*dtr))
#define store_idt(dtr) __asm__ __volatile__("sidt %0":"=m" (*dtr))
#define store_tr(tr) __asm__ __volatile__("str %0":"=mr" (tr))
#define store_ldt(ldt) __asm__ __volatile__("sldt %0":"=mr" (ldt))

#define _set_tssldt_desc(n,addr,limit,type) \
__asm__ __volatile__ ("movw %w3,0(%2)\n\t" \
	"movw %w1,2(%2)\n\t" \
	"rorl $16,%1\n\t" \
	"movb %b1,4(%2)\n\t" \
	"movb %4,5(%2)\n\t" \
	"movb $0,6(%2)\n\t" \
	"movb %h1,7(%2)\n\t" \
	"rorl $16,%1" \
	: "=m"(*(n)) : "q" (addr), "r"(n), "ir"(limit), "i"(type))


static inline void __set_tss_desc(unsigned int cpu, unsigned int entry, void *addr){
  _set_tssldt_desc(&get_cpu_gdt_table(cpu)[entry], (int)addr,
		   offsetof(struct tss_struct, __cacheline_filler) - 1, 0x89);
}
#define set_tss_desc(cpu,addr) __set_tss_desc(cpu, GDT_ENTRY_TSS, addr)
#endif
