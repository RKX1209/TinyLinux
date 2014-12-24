/*
 * arch/i386/kernel/setup.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/bootmem.h>
#include <abyon/mmzone.h>
#include <drivers/disp.h>

#include <asm/e820.h>
#include <asm/pgtable.h>

extern int printk(const char *fmt, ...);
extern unsigned long _fin_end;
void paging_init(void);
unsigned long init_pg_tables_end = ~0UL;
  
void init_e820(){
  e820.nr_map = 7;
  e820.map[0] = (struct e820entry){ 0x00000000,0x0009ffff, E820_RAM };
  e820.map[1] = (struct e820entry){ 0x000f0000,65536, E820_RESERVED };
  e820.map[2] = (struct e820entry){ 0x00100000,133103616, E820_RAM };
  e820.map[3] = (struct e820entry){ 0x07ff0000,125841408, E820_ACPI };
  e820.map[4] = (struct e820entry){ 0x07ff3000,53248, E820_ACPI };
  e820.map[5] = (struct e820entry){ 0x08000000,4160684032, E820_RAM };
  e820.map[6] = (struct e820entry){ 0xffff0000,65536, E820_RESERVED };
}

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
    printk("Clearing... e820[%d](%d,%d)",i,cur_pfn,last_pfn);
    free_bootmem(cur_pfn << PAGE_SHIFT,size << PAGE_SHIFT);
  }
}

/* Setup bootmem allocator */
void setup_bootmem_allocator(void){
  unsigned long bootmap_size = init_bootmem(min_low_pfn,max_low_pfn);
  /* free all 'enable' pages */
  register_bootmem_low_pages(max_low_pfn);
  printk("Free bootmem pages");
  /* reserve memory map itself */
  reserve_bootmem(min_low_pfn << PAGE_SHIFT,
  		  ((min_low_pfn << PAGE_SHIFT) + bootmap_size + PAGE_SIZE - 1) - (min_low_pfn << PAGE_SHIFT));

  //printk("reserve bit table[%d,]",min_low_pfn);
  reserve_bootmem(__PAGE_OFFSET,
  		  ((unsigned long)&(_fin_end) - __PAGE_OFFSET));
  //printk("reserve kernel[%d,%d]",__PAGE_OFFSET >> PAGE_SHIFT,(unsigned long)(&_fin_end) >> PAGE_SHIFT);
  reserve_bootmem(0,PAGE_SIZE);
  //printk("reserve BIOS[%d,%d]",0,PAGE_SIZE >> PAGE_SHIFT);

/* reserve BIOS EBDA region */
  reserve_bootmem(0x40E,PAGE_SIZE);
  //printk("reserve EBDA[%d,%d]",0x40e >> PAGE_SHIFT,(0x40e + PAGE_SIZE) >> PAGE_SHIFT);
}

static unsigned long setup_memory(void){
  init_e820();
  //printk("init_e820");
  
  min_low_pfn = (e820.map[0].addr + 0x7e00) >> PAGE_SHIFT;
  max_low_pfn = (e820.map[6].addr +  e820.map[6].size - 1) >> PAGE_SHIFT;
  
  setup_bootmem_allocator();
  return max_low_pfn;
}
/* setup arch part */
void setup_arch(){
  printk("setup arch");
  /* Setup memory informations */
  setup_memory();

  /* Setup page tables */  
  paging_init();
  printk("setup arch ....[OK]");
}
