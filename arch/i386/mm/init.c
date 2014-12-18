/*
 * arch/i386/mm/init.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/bootmem.h>
#include <abyon/init_task.h>
#include <abyon/sched.h>

#include <asm/system.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/fixmap.h>
#include <asm/highmem.h>
#include <asm/tlbflush.h>

extern int printk(const char *fmt, ...);

static inline int is_kernel_text(unsigned long addr){
  //if(PAGE_OFFSET <= addr && addr <= (unsigned long)_text_end)
  return 1;
}
static unsigned long i;
static pte_t *one_page_table_init(pgd_t *pgd){
  if(pgd_none(*pgd)){

    pte_t *page_table = (pte_t *) alloc_bootmem_low_pages(PAGE_SIZE);
    unsigned long ph_addr = (unsigned long)page_table;//Straight map area
    //printk("alloc pgd[%d] (0x%x)",i++,ph_addr);
    pgd->pgd = (ph_addr | _PAGE_TABLE);
    return page_table;
  }
  return pgd_pa_addr(*pgd,0);
}

static void page_table_range_init(unsigned long start,unsigned long end,pgd_t *pgd_base){
  unsigned long vaddr = start;
  int pgd_idx = pgd_index(vaddr);
  pgd_t *pgd = pgd_base + pgd_idx;
  for(; (pgd_idx < PTRS_PER_PGD) && (vaddr < end); pgd++,pgd_idx++){
    if(pgd_none(*pgd)){
      printk("none");
      one_page_table_init(pgd);
    }
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

static void kernel_physical_mapping_init(pgd_t *pgd_base){
  pgd_t *pgd;
  pte_t *pte;  
  int pgd_idx,pte_idx;

  pgd = pgd_base;
  printk("Setting memory tables...");

  unsigned long address;
  unsigned long pfn = 0;
  for(pfn = 0; pfn < max_low_pfn; pfn++){
    address = pfn * PAGE_SIZE;
    pgd_idx = pgd_index(address);
    pte_idx = pte_index(address);
    
    pte = one_page_table_init(&pgd[pgd_idx]);
    pte_t* pte_entry = &(pte[pte_idx]);
    
    if(is_kernel_text(address))
      set_pte(pte_entry,pfn_pte(pfn,PAGE_KERNEL_EXEC));
    else
      set_pte(pte_entry,pfn_pte(pfn,PAGE_KERNEL_EXEC));
    
    if(pfn < 2){
      printk("virt:0x%x->pgd[%d](0x%x)=(0x%x)",address,pgd_idx,&pgd[pgd_idx],pgd[pgd_idx].pgd);
      printk("=>pte(0x%x) pte[%d](0x%x) ph[0x%x]",pte,pte_idx,pte_entry,pte_entry->pte_low);
    }
    
  }
  printk("Setting memory tables... [OK]");
}


static void pagetable_init(void){
  /* Set page tables of "straight map area" */
  pgd_t *pgd_base = (pgd_t*)swapper_pg_dir;
  kernel_physical_mapping_init(pgd_base);  

  
  /* Set page tables of "fix map area" */
  unsigned long fixed_vaddr = __fix_to_virt(__end_of_fixed_addresses - 1) & PGDIR_MASK;
  page_table_range_init(fixed_vaddr,0,pgd_base);

  /* Set page tables of "HIGHMEM area" */
  //permanent_kmaps_init(pgd_base);
  return;
}

void paging_init(void){
  pagetable_init();
  pgd_t* pgd_base = (pgd_t*)swapper_pg_dir;
  unsigned long pfn;
  
  write_cr3(pgd_base);
  unsigned long cr3i = read_cr3();
  unsigned long cr4i = read_cr4();
  unsigned long cr0i = read_cr0();
  cr0i |= 0x80000000;
  write_cr0(cr0i);  
  __flush_tlb_all();
  //kmap_init();  
}
