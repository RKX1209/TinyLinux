/*
 * arch/i386/kernel/setup.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/bootmem.h>
#include <asm/e820.h>
#include <asm/pgtable.h>

void paging_init(void);
unsigned long init_pg_tables_end = ~0UL;
  

/* Enable bootmem low pages */
static void register_bootmem_low_pages(unsigned long max_low_pfn){
  int i;
  for(i = 0; i < e820.nr_map; i++){
    if(e820.map[i].type != E820_RAM) continue;

    unsigned long cur_pfn = PFN_UP(e820.map[i].addr);
    if(cur_pfn >= max_low_pfn) continue;

    unsigned long last_pfn = PFN_DOWN(e820.map[i].addr + e820.map[i].size);
    if (last_pfn > max_low_pfn)
      last_pfn = max_low_pfn;

    if (last_pfn <= cur_pfn)
      continue;

    unsigned long size = last_pfn - cur_pfn;
    free_bootmem(cur_pfn << PAGE_SHIFT,size << PAGE_SHIFT);
  }
}

/* Setup bootmem allocator */
void setup_bootmem_allocator(void){
  unsigned long bootmap_size = init_bootmem(min_low_pfn,max_low_pfn);
  register_bootmem_low_pages(max_low_pfn);

reserve_bootmem(__PHYSICAL_START,((min_low_pfn << PAGE_SIZE) + bootmap_size + PAGE_SIZE - 1) - (__PHYSICAL_START));
reserve_bootmem(0,PAGE_SIZE);

/* reserve BIOS EBDA region */
reserve_bootmem(0x40E,PAGE_SIZE);
}

static unsigned long setup_memory(void){
  setup_bootmem_allocator();
  return max_low_pfn;
}
/* setup arch part */
void setup_arch(){

  /* Setup memory informations */
  unsigned long max_low_pfn = setup_memory();
  
  /* Setup page tables */  
  paging_init();
}
