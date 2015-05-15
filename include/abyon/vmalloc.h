/*
 * include/abyon/vmalloc.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _I386_VMALLOC_H
#define _I386_VMALLOC_H

#define VM_IOREMAP	0x00000001	/* ioremap() and friends */
#define VM_ALLOC	0x00000002	/* vmalloc() */
#define VM_MAP		0x00000004	/* vmap()ed pages */

struct vm_struct{
  void *addr;
  unsigned long size;
  unsigned long flags;
  struct page **pages;
  unsigned int nr_pages;
  unsigned long phys_addr;
  struct vm_struct *next;
};

#endif
