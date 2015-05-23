/*
 * kernel/pgtable.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/sched.h>
#include <abyon/slab.h>

#include <asm/pgtable.h>

extern kmem_cache_t *pgd_cache;
pgd_t *pgd_alloc(struct mm_struct *mm){
  pgd_t *pgd = kmem_cache_alloc(pgd_cache,0);
  int i;
  for(i = 0; i < PTRS_PER_PGD; ++i){
    
  }
  return pgd;
}
