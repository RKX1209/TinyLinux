/*
 * include/asm/processor.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef __ASM_I386_PROCESSOR_H
#define __ASM_I386_PROCESSOR_H

#include <abyon/percpu.h>

#include <asm/segment.h>
#include <asm/system.h>

struct desc_struct {
  unsigned long a,b;
};

DECLARE_PER_CPU(struct tss_struct, init_tss);

#define IO_BITMAP_BITS  65536
#define IO_BITMAP_BYTES (IO_BITMAP_BITS/8)
#define IO_BITMAP_LONGS (IO_BITMAP_BYTES/sizeof(long))
#define IO_BITMAP_OFFSET offsetof(struct tss_struct,io_bitmap)
#define INVALID_IO_BITMAP_OFFSET 0x8000
#define INVALID_IO_BITMAP_OFFSET_LAZY 0x9000

struct tss_struct {
  unsigned short back_link,__blh;
  unsigned long	esp0;
  unsigned short ss0,__ss0h;
  unsigned long	esp1;
  unsigned short ss1,__ss1h;	/* ss1 is used to cache MSR_IA32_SYSENTER_CS */
  unsigned long	esp2;
  unsigned short ss2,__ss2h;
  unsigned long	__cr3;
  unsigned long	eip;
  unsigned long	eflags;
  unsigned long	eax,ecx,edx,ebx;
  unsigned long	esp;
  unsigned long	ebp;
  unsigned long	esi;
  unsigned long	edi;
  unsigned short es, __esh;
  unsigned short cs, __csh;
  unsigned short ss, __ssh;
  unsigned short ds, __dsh;
  unsigned short fs, __fsh;
  unsigned short gs, __gsh;
  unsigned short ldt, __ldth;
  unsigned short trace, io_bitmap_base;
  unsigned long	io_bitmap[IO_BITMAP_LONGS + 1];
  unsigned long io_bitmap_max;
  struct thread_struct *io_bitmap_owner;
  unsigned long __cacheline_filler[35];
  unsigned long stack[64];
} __attribute__((packed));

struct thread_struct {
  struct desc_struct tls_array[GDT_ENTRY_TLS_ENTRIES];
  unsigned long	esp0;
  unsigned long	sysenter_cs;
  unsigned long	eip;
  unsigned long	esp;
  unsigned long	fs;
  unsigned long	gs;
  unsigned long	debugreg[8];  /* %%db0-7 debug registers */
  unsigned long	cr2, trap_no, error_code;
  unsigned long		screen_bitmap;
  unsigned long		v86flags, v86mask, saved_esp0;
  unsigned int		saved_fs, saved_gs;
  unsigned long	*io_bitmap_ptr;
  unsigned long	iopl;
  unsigned long	io_bitmap_max;
};

#define INIT_THREAD  {							\
	.io_bitmap_ptr = NULL,						\
}

static inline void load_esp0(struct tss_struct *tss, struct thread_struct *thread){
  tss->esp0 = thread->esp0;
}
#endif
