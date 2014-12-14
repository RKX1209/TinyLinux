/*
 * include/asm/segment.h
 * Copyright (C) <2014>  <@RKX1209>
 */


#ifndef _ASM_SEGMENT_H
#define _ASM_SEGMENT_H

/* GDT entry (Kernel space) */
#define GDT_ENTRY_KERNEL_BASE 12


#define GDT_ENTRY_KERNEL_CS (GDT_ENTRY_KERNEL_BASE + 0)//Kernel CS
#define __KERNEL_CS (GDT_ENTRY_KERNEL_CS * 8)

#define GDT_ENTRY_KERNEL_DS (GDT_ENTRY_KERNEL_BASE + 1)//Kernel DS
#define __KERNEL_DS (GDT_ENTRY_KERNEL_DS * 8)

/* GDT Size */
#define GDT_ENTRIES 32
#define GDT_SIZE (GDT_ENTRIES * 8)

/* Simple GDT entries used in booting process */
#define GDT_ENTRY_BOOT_CS		2
#define __BOOT_CS	(GDT_ENTRY_BOOT_CS * 8)

#define GDT_ENTRY_BOOT_DS		(GDT_ENTRY_BOOT_CS + 1)
#define __BOOT_DS	(GDT_ENTRY_BOOT_DS * 8)

/* IDT Size */
#define IDT_ENTRIES 256

#endif
