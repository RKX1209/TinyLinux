/*
 * mm/bootmem.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/bootmem.h>
#include <abyon/kernel.h>

#include <asm/bitops.h>
#include <asm/string.h>

#define BITS_PER_LONG 32

unsigned long max_low_pfn;
unsigned long min_low_pfn;

extern fastcall void __free_pages(struct page *page,unsigned int order);
extern int find_next_zero_bit(const unsigned long *addr, int size, int offset);
extern int printk(const char *fmt, ...);

static unsigned long init_bootmem_core (pg_data_t *pgdat,unsigned long mapstart,
					unsigned long start, unsigned long end){
  
  bootmem_data_t *bdata = pgdat->bdata;
  unsigned long mapsize = ((end - start) + 7) / 8;
  pgdat->pgdat_next = pgdat_list;
  pgdat_list = pgdat;
  
  mapsize = (mapsize + (sizeof(long) - 1UL)) & ~(sizeof(long) - 1UL);
  bdata->node_bootmem_map = (void *)(mapstart << PAGE_SHIFT); /* Set free area table */
  bdata->node_boot_start = (start << PAGE_SHIFT);
  bdata->node_end_pfn = end;
  __memset(bdata->node_bootmem_map,0xff,mapsize);
  printk("bit table [0x%x - 0x%x]",mapstart << PAGE_SHIFT,(mapstart << PAGE_SHIFT) + mapsize);
  //printk("map: [0x%x - 0x%x]",start << PAGE_SHIFT,end << PAGE_SHIFT);
  printk("map: [%d - %d]",start,end);
  return mapsize;
  
}

static void reserve_bootmem_core(bootmem_data_t *bdata, unsigned long addr,
				 unsigned long size){

  unsigned long sidx = (addr - bdata->node_boot_start) / PAGE_SIZE;
  unsigned long eidx = (addr + size - bdata->node_boot_start + PAGE_SIZE - 1) / PAGE_SIZE;
  //printk("reserved [0x%x,0x%x]",sidx << PAGE_SHIFT,eidx << PAGE_SHIFT);
  //unsigned long end = (addr + size + PAGE_SIZE - 1) / PAGE_SIZE;
  unsigned long i;
  for(i = sidx; i < eidx; i++){
    test_and_set_bit(i,bdata->node_bootmem_map);
  }
}

void * __alloc_bootmem_core(struct bootmem_data *bdata, unsigned long size,
			    unsigned long align, unsigned long goal){

  unsigned long offset, remaining_size, areasize, preferred = 0;
  unsigned long i, j,start = 0, incr, eidx;
  void *ret;

  eidx = bdata->node_end_pfn - (bdata->node_boot_start >> PAGE_SHIFT);

  offset = 0;
  if (align &&(bdata->node_boot_start & (align - 1UL)) != 0)
    offset = (align - (bdata->node_boot_start & (align - 1UL)));
  offset >>= PAGE_SHIFT;

  if (goal && (goal >= bdata->node_boot_start) &&
      ((goal >> PAGE_SHIFT) < bdata->node_end_pfn)) {
    preferred = goal - bdata->node_boot_start;
    if (bdata->last_success >= preferred)
      preferred = bdata->last_success;

  } else
    preferred = 0;
  preferred = ((preferred + align - 1) & ~(align - 1)) >> PAGE_SHIFT;
  preferred += offset;

  areasize = (size+PAGE_SIZE-1)/PAGE_SIZE;

  incr = align >> PAGE_SHIFT ? : 1;

 restart_scan:
  //printk("trying alloc[0x%x-0x%x]",preferred << PAGE_SHIFT,eidx << PAGE_SHIFT);
  //printk("trying alloc[%d-%d]",preferred,eidx);
  for (i = preferred; i < eidx; i += incr) {
    //i = find_next_zero_bit(bdata->node_bootmem_map, eidx, i);
    i = ALIGN(i, incr);
    if (test_bit(i, bdata->node_bootmem_map))
      continue;
    for (j = i + 1; j < i + areasize; ++j) {
      if (j >= eidx)
	goto fail_block;
      if (test_bit (j, bdata->node_bootmem_map))
	goto fail_block;
    }
    start = i;
    goto found;
  fail_block:
    i = ALIGN(j, incr);    
  }
  if (preferred > offset) {
    preferred = offset;
    goto restart_scan;
  }
  return 0;

 found:
  //printk("Alloc%d",start);
  bdata->last_success = start << PAGE_SHIFT;
  
  if (align < PAGE_SIZE && bdata->last_offset && bdata->last_pos+1 == start) {
    offset = (bdata->last_offset+align-1) & ~(align-1);
    remaining_size = PAGE_SIZE-offset;
    if (size < remaining_size) {
      areasize = 0;
      /* last_pos unchanged */
      bdata->last_offset = offset+size;
      ret = (void*)(bdata->last_pos*PAGE_SIZE + offset + bdata->node_boot_start);
    } else {
      remaining_size = size - remaining_size;
      areasize = (remaining_size+PAGE_SIZE-1)/PAGE_SIZE;
      ret = (void*)(bdata->last_pos*PAGE_SIZE + offset + bdata->node_boot_start);
      bdata->last_pos = start+areasize-1;
      bdata->last_offset = remaining_size;
    }
    bdata->last_offset &= ~PAGE_MASK;
  } else {
    bdata->last_pos = start + areasize - 1;
    bdata->last_offset = size & ~PAGE_MASK;
    ret = (void*)(start * PAGE_SIZE + bdata->node_boot_start);
  }

  for (i = start; i < start+areasize; i++)
    test_and_set_bit(i, bdata->node_bootmem_map);
  memset(ret, 0, size);

  return ret;  
}

static unsigned long free_all_bootmem_core(pg_data_t *pgdat){
  bootmem_data_t *bdata = pgdat->bdata;
  unsigned long pfn = bdata->node_boot_start >> PAGE_SHIFT;
  unsigned long idx = bdata->node_end_pfn - pfn;
  unsigned long *map = bdata->node_bootmem_map;
  unsigned long i,j,count = 0,total = 0,order;
  struct page *page;
  printk("bootmem_fin: %d\n",idx);
  for(i = 0; i < idx; ){
    unsigned long v = ~map[i / BITS_PER_LONG];
    if(v == ~0UL){
      page = pfn_to_page(pfn);
      count += BITS_PER_LONG;
      __clear_bit(PG_reserved,&page->flags);
      order = ffs(BITS_PER_LONG) - 1;
      page->_count = 0;
      for(j = 1; j < BITS_PER_LONG; j++){
  	if(j + 16 < BITS_PER_LONG);
  	__clear_bit(PG_reserved,&(page + j)->flags);
  	(page + j)->_count = 0;
      }
      __free_pages(page,order);
      i += BITS_PER_LONG;
      page += BITS_PER_LONG;
    }else if(v){
      page = pfn_to_page(pfn);
      unsigned long m;
      for(m = 1; m && i < idx; m <<= 1, page++,i++){
  	if(v & m){
  	  count++;
  	  __clear_bit(PG_reserved,&page->flags);
  	  page->_count = 0;
  	  __free_pages(page,0);
  	}
      }
    }else{
      i += BITS_PER_LONG;
    }
    pfn += BITS_PER_LONG;
  }
  total += count;
  page = virt_to_page((unsigned long)(bdata->node_bootmem_map));
  count = 0;
  for(i = 0; i < ((bdata->node_end_pfn - (bdata->node_boot_start >> PAGE_SHIFT)) / 8 + PAGE_SIZE - 1) / PAGE_SIZE; i++,page++){
    count++;
    __clear_bit(PG_reserved,&page->flags);
    page->_count = 0;
    __free_pages(page,order);
  }
  total += count;
  bdata->node_bootmem_map = 0;
  return total;
}

unsigned long free_all_bootmem_node(pg_data_t *pgdat){
  return (free_all_bootmem_core(pgdat));
}

static void free_bootmem_core(bootmem_data_t *bdata,
			      unsigned long addr, unsigned long size){
  /* bdata management range (index) */

  unsigned long start = (addr + PAGE_SIZE - 1) / PAGE_SIZE;
  unsigned long end = (addr + size) / PAGE_SIZE;

  unsigned long sidx = start - (bdata->node_boot_start / PAGE_SIZE);
  unsigned long eidx = (addr + size - bdata->node_boot_start) / PAGE_SIZE;

  if(addr < bdata->last_success) bdata->last_success = addr;
  
  unsigned long i;
  for(i = sidx; i < eidx; i++){
    if(!test_and_clear_bit(i,bdata->node_bootmem_map))
      printk("[!!]Cannot clear map[%d]",i);
  }
  printk("Cleared... e820[0x%x](%d,%d) [OK]",bdata->node_bootmem_map,sidx,eidx);
}

unsigned long init_bootmem (unsigned long start, unsigned long pages){
  NODE_DATA(0)->bdata = &bdata;
  return (init_bootmem_core(NODE_DATA(0),start,0,pages));
}

unsigned long init_bootmem_node (pg_data_t *pgdat, unsigned long freepfn,
				 unsigned long startpfn, unsigned long endpfn) {
  return(init_bootmem_core(pgdat, freepfn, startpfn, endpfn));
}

void reserve_bootmem (unsigned long addr, unsigned long size)
{
  reserve_bootmem_core(NODE_DATA(0)->bdata, addr, size);
}

void free_bootmem (unsigned long addr, unsigned long size)
{
  free_bootmem_core(NODE_DATA(0)->bdata, addr, size);
}
