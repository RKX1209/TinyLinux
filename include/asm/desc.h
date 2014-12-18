/*
 * include/asm/desc.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef __ASM_DESC_H
#define __ASM_DESC_H

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

#define load_gdt(dtr) __asm__ __volatile("lgdt %0"::"m" (*dtr))
#define load_idt(dtr) __asm__ __volatile("lidt %0"::"m" (*dtr))
#define load_tr(tr) __asm__ __volatile("ltr %0"::"mr" (tr))
#define load_ldt(ldt) __asm__ __volatile("lldt %0"::"mr" (ldt))

#define store_gdt(dtr) __asm__ ("sgdt %0":"=m" (*dtr))
#define store_idt(dtr) __asm__ ("sidt %0":"=m" (*dtr))
#define store_tr(tr) __asm__ ("str %0":"=mr" (tr))
#define store_ldt(ldt) __asm__ ("sldt %0":"=mr" (ldt))

#endif
