/*
 * include/asm/page.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _I386_PAGE_H
#define _I386_PAGE_H

/* Page size(4k) */
#define PAGE_SHIFT	12
#define PAGE_SIZE	(1UL << PAGE_SHIFT)
#define PAGE_MASK       (~(PAGE_SIZE - 1))

/* Virtual page tables */
typedef struct { unsigned long pte_low; } pte_t;
typedef struct { unsigned long pgd; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;


/* Kernel entry address */
#define __PAGE_OFFSET		(0x00100000)
#define __PHYSICAL_START        (0x00100000)
#define PAGE_OFFSET		((unsigned long)__PAGE_OFFSET)

#endif
