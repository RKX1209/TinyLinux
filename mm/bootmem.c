/*
 * mm/bootmem.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/bootmem.h>
#include <abyon/mmzone.h>

#include <asm/bitops.h>

unsigned long max_low_pfn;
unsigned long min_low_pfn;

static unsigned long init_bootmem_core (pg_data_t *pgdat,unsigned long mapstart,
					unsigned long start, unsigned long end){
  
  bootmem_data_t *bdata = pgdat->bdata;
  unsigned long mapsize = ((end - start) + 7) / 8;
  pgdat->pgdat_next = pgdat_list;
  pgdat_list = pgdat;
  
  mapsize = (mapsize + (sizeof(long) - 1UL)) & ~(sizeof(long) - 1UL);
  bdata->node_bootmem_map = mapstart << PAGE_SHIFT; /* Set free area table */
  
  bdata->node_boot_start = (start << PAGE_SHIFT);
  bdata->node_end_pfn = end;
  memset(bdata->node_bootmem_map,0xff,mapsize);
  return mapsize;
  
}

static void reserve_bootmem_core(bootmem_data_t *bdata, unsigned long addr,
				 unsigned long size){
  unsigned long sidx = (addr - bdata->node_boot_start) / PAGE_SIZE;
  unsigned long eidx = (addr + size - bdata->node_boot_start + PAGE_SIZE - 1) / PAGE_SIZE;
  //unsigned long end = (addr + size + PAGE_SIZE - 1) / PAGE_SIZE;
  unsigned long i;
  for(i = sidx; i < eidx; i++){
    test_and_set_bit(i,bdata->node_bootmem_map);
  }
}

static void *  __alloc_bootmem_core(struct bootmem_data *bdata, unsigned long size,
				    unsigned long align, unsigned long goal){
  unsigned long start = 0;
  unsigned long eidx = bdata->node_end_pfn - (bdata->node_boot_start >> PAGE_SHIFT);
  unsigned long offset = 0;
  if(align &&(bdata->node_boot_start & (align - 1UL)) != 0)    
    offset = (align - (bdata->node_boot_start & (align - 1UL)));

  unsigned long preferred;
  if(goal && (goal >= bdata->node_boot_start) &&
     ((goal >> PAGE_SHIFT) < bdata->node_end_pfn)){
    preferred = goal - bdata->node_boot_start;
    if(bdata->last_success >= preferred) preferred = bdata->last_success;    
  }else{
    preferred = 0;
  }
  preferred = ((preferred + align - 1) & ~(align - 1)) >> PAGE_SHIFT;
  preferred += offset;
  unsigned long areasize = (size + PAGE_SIZE - 1) / PAGE_SIZE;
  unsigned long incr = align >> PAGE_SHIFT ? : 1;

 restart_scan:;
  unsigned long i;
  for(i = preferred; i < eidx; i+= incr){
    i = find_next_zero_bit(bdata->node_bootmem_map,eidx,i);
    i = ALIGN(i,incr);
    if(test_bit(i,bdata->node_bootmem_map)) continue;
    unsigned long j;
    for(j = i + 1; j < i + areasize; j++){
      if(j >= eidx) goto fail_block;
      if(test_bit(j,bdata->node_bootmem_map)) goto fail_block;
    }
    start = i;
    goto found;
  fail_block:
    i = ALIGN(j,incr);
  }
  if(preferred > offset){
    preferred = offset;
    goto restart_scan;
  }
  return 0;

 found:
  bdata->last_success = start << PAGE_SHIFT;
  bdata->last_pos = start + areasize - 1;
  bdata->last_offset &= ~PAGE_MASK;
  for(i = start; i < start + areasize; i++)
    test_and_set_bit(i,bdata->node_bootmem_map);
  memset(start,0,size);
  return start;
}
static void free_bootmem_core(bootmem_data_t *bdata,
			      unsigned long addr, unsigned long size){
  /* bdata management range (index) */
  unsigned long sidx;
  unsigned long eidx = (addr + size - bdata->node_boot_start) / PAGE_SIZE;

  unsigned long end = (addr + size) / PAGE_SIZE;
  unsigned long start = (addr + PAGE_SIZE - 1) / PAGE_SIZE;
  if(addr < bdata->last_success) bdata->last_success = addr;
  sidx = start - (bdata->node_boot_start / PAGE_SIZE);
  unsigned long i;
  for(i = sidx; i < eidx; i++){
    test_and_clear_bit(i,bdata->node_bootmem_map);
  }
}

unsigned long init_bootmem (unsigned long start, unsigned long pages){
  max_low_pfn = pages;
  min_low_pfn = start;
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
