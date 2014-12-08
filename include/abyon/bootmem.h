/*
 * include/abyon/bootmem.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ABYON_BOOTMEM_H
#define _ABYON_BOOTMEM_H

#include <abyon/mmzone.h>

#include <asm/pgtable.h>


extern unsigned long max_low_pfn;
extern unsigned long min_low_pfn;

extern unsigned long max_pfn;

typedef struct bootmem_data{
  unsigned long node_boot_start;/* Start (physical) address */
  unsigned long node_end_pfn;
  void *node_bootmem_map; /* Bitset of free areas(0:free, 1:used) */
  unsigned long last_offset;
  unsigned long last_pos;
  unsigned long last_success;
} bootmem_data_t;

extern unsigned long init_bootmem (unsigned long addr, unsigned long memend);
extern unsigned long init_bootmem_node (pg_data_t *pgdat, unsigned long freepfn,
					unsigned long startpfn, unsigned long endpfn);
extern void *  __alloc_bootmem_core(struct bootmem_data *bdata, unsigned long size,
				    unsigned long align, unsigned long goal);
extern void free_bootmem (unsigned long addr, unsigned long size);
extern void reserve_bootmem (unsigned long addr, unsigned long size);

#define alloc_bootmem_low_pages(x) \
  __alloc_bootmem((x), PAGE_SIZE, 0)

static inline void *__alloc_bootmem (unsigned long size, unsigned long align, unsigned long goal)
{
  pg_data_t *pgdat = pgdat_list;
  return __alloc_bootmem_core(pgdat->bdata,size, align, goal);
}



#endif
