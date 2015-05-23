/*
 * mm/memory.c
 * Copyright (C) <2015>  <@RKX1209>
 */
#include <abyon/sched.h>
#include <abyon/mm.h>

#include <asm/pgtable.h>

int copy_page_range(struct mm_struct *dst, struct mm_struct *src,
		    struct vm_area_struct *vma){
  unsigned long next;
  unsigned long address = vma->vm_start;
  unsigned long end = vma->vm_end;
  pgd_t *dst_pgd = pgd_offset(dst,address);
  pgd_t *src_pgd = pgd_offset(src,address);
  do{
    
  }while(dst_pgd++,src_pgd++,address = next,address != end);
  return 0;
}
