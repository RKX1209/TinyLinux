/*
 * mm/vmalloc.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <asm/pgtable.h>

struct vm_struct *vmlist;

int map_vm_area(struct vm_struct *area, unsigned long prot,struct page ***pages){
  unsigned long address = (unsigned long) area->addr;
  unsigned long end = address + (area->size-PAGE_SIZE);
  pgd_t *dir = pgd_offset_k(address);
  do{    
    dir++;
  }while(address && (address < end));
  return 1;
}

struct vm_struct *__get_vm_area(unsigned long size,unsigned long flags,
				unsigned long start,unsigned long end){
  unsigned long addr = start;
  struct vm_struct *area = kmalloc(sizeof(*area),0);
  if(!area) return 0;
  size += PAGE_SIZE;
  struct vm_struct **p,*tmp;
  for(p = &vmlist; (tmp = *p) != 0; p = &tmp->next){
    if((size + addr) < addr) goto out;
    if(size + addr <= (unsigned long)tmp->addr) goto found;
  }
 found:
  area->next = *p;
  *p = area;
  area->flags = flags;
  area->addr = (void*)addr;
  area->size = size;
  area->pages = 0;
  area->nr_pages = 0;
  area->phys_addr = 0;
  
  return area;
}
struct vm_struct *get_vm_area(unsigned long size,unsigned long flags){
  return __get_vm_area(size,flags,VMALLOC_START,VMALLOC_END);
}

void *__vmalloc(unsigned long size, int gfp_mask, unsigned long prot){
  /* Get free space from [VMALLOC_START,VMALLOC_END] */
  struct vm_struct *area = get_vm_area(size,VM_ALLOC);
  struct page **pages;
  if(!area) return 0;
  unsigned int nr_pages = size >> PAGE_SHIFT;
  unsigned array_size = (nr_pages * sizeof(struct page*));
  area->nr_pages = nr_pages;
  area->pages = pages = kmalloc(array_size,0);  
  memset(area->pages,0,array_size);
  unsigned long i;
  for(i = 0; i < area->nr_pages; i++){
    area->pages[i] = alloc_page(0);    
  }
  if(map_vm_area(area,prot,&pages)) goto fail;
  return area->addr;

 fail:
  vfree(area->addr);
  return 0;
}

void *vmalloc(unsigned long size){
  return __vmalloc(size,0,0);
}

void unmap_vm_area(struct vm_struct *area){
  unsigned long address = (unsigned long)area->addr;
  unsigned long end = (address + area->size);
  pgd_t *dir = pgd_offset_k(address);
  do{
    dir++;
  }while(address && (address <end));
}

struct vm_struct *remove_vm_area(void *addr){
  struct vm_struct **p,*tmp;
  for(p = &vmlist; (tmp = *p) != 0; p = &tmp->next){
    if(tmp->addr == addr) goto found;
  }
  return 0;
 found:
  unmap_vm_area(tmp);
  *p = tmp->next;
  return tmp;
}

void __vunmap(void *addr, int deallocate_pages){
  if(!addr) return;
  struct vm_struct *area = remove_vm_area(addr);
  kfree(area);
  return;
}

void vfree(void *addr){
  __vunmap(addr,1);
}
