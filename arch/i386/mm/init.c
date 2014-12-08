/*
 * arch/i386/mm/init.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/bootmem.h>

#include <asm/system.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/fixmap.h>
#include <asm/highmem.h>

static inline int is_kernel_text(unsigned long addr){
  //if(PAGE_OFFSET <= addr && addr <= (unsigned long)_text_end)
  return 1;
}

static pte_t *one_page_table_init(pgd_t *pgd){
  if(pgd_none(pgd)){
    pte_t *page_table = (pte_t *) alloc_bootmem_low_pages(PAGE_SIZE);
    //pte_t *page_table;
    unsigned long ph_addr = (unsigned long)page_table - PAGE_OFFSET;//Straight map area      
    pgd = &((pgd_t){ph_addr | _PAGE_TABLE});
    return page_table;
  }
  return pte_virtual_addr(pgd,0);
}

static void page_table_range_init(unsigned long start,unsigned long end,pgd_t *pgd_base){
  unsigned long vaddr = start;
  int pgd_idx = pgd_index(vaddr);
  pgd_t *pgd = pgd_base + pgd_idx;
  for(; (pgd_idx < PTRS_PER_PGD) && (vaddr != end); pgd++,pgd_idx++){
    if(pgd_none(*pgd))
      one_page_table_init(pgd);
    vaddr += PGDIR_SIZE;
  }
}

pte_t *kmap_pte;
pgprot_t kmap_prot;

static void kmap_init(void){
  unsigned long kmap_vstart = __fix_to_virt(FIX_KMAP_BEGIN);
  kmap_pte = pte_virtual_addr(pgd_offset_k(kmap_vstart),kmap_vstart);
  kmap_prot = PAGE_KERNEL;
}

static void permanent_kmaps_init(pgd_t *pgd_base){
  pgd_t *pgd;
  pte_t *pte;
  unsigned long vaddr = PKMAP_BASE;
  page_table_range_init(vaddr,vaddr + PAGE_SIZE * LAST_PKMAP,pgd_base);

  pgd = swapper_pg_dir + pgd_index(vaddr);
  pte = pte_virtual_addr(pgd,vaddr);
  pte_t *pkmap_page_table = pte;	
}

static void kernel_physical_mapping_init(pgd_t *pgd_base) {
  pgd_t *pgd;
  pte_t *pte;

  int pgd_idx,pte_idx;
  pgd_idx = pgd_index(PAGE_OFFSET);
  pgd = pgd_base + pgd_idx;
  unsigned long pfn; //page frame number(simply 4k-page number)
  pfn = 0;
  for(; pgd_idx < PTRS_PER_PGD; pgd++,pgd_idx++){
    if(pfn >= max_low_pfn) continue;
    unsigned long address = pfn * PAGE_SIZE + PAGE_OFFSET;
    pte = one_page_table_init(pgd);
    for(pte_idx = 0; pte_idx < PTRS_PER_PTE && pfn < max_low_pfn; pte++,pfn++,pte_idx++){
      if(is_kernel_text(address))
	set_pte(pte,pfn_pte(pfn,PAGE_KERNEL_EXEC));
      else
	set_pte(pte,pfn_pte(pfn,PAGE_KERNEL));
    }
  }
}


static void pagetable_init(void){
  /* Set page tables of "straight map area" */
  pgd_t *pgd_base = swapper_pg_dir;
  kernel_physical_mapping_init(pgd_base);  

  /* Set page tables of "fix map area" */
  unsigned long fixed_vaddr = __fix_to_virt(__end_of_fixed_addresses - 1) & PGDIR_MASK;
  page_table_range_init(fixed_vaddr,0,pgd_base);

  /* Set page tables of "HIGHMEM area" */  
  permanent_kmaps_init(pgd_base);
  return;
}

void paging_init(void){
  pagetable_init();
  write_cr0(swapper_pg_dir);
  //__flush_tlb_all();
  kmap_init();  
}
