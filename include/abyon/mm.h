/*
 * include/abyon/mm.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ABYON_MM_H
#define _ABYON_MM_H

#include <abyon/list.h>
#include <abyon/mmzone.h>
#include <abyon/rbtree.h>

struct page{
  unsigned long flags;
  
  unsigned long _count;
  unsigned long private;
  struct list_head lru;
};

#define PG_locked	 	 0	/* Page is locked. Don't touch. */
#define PG_error		 1
#define PG_referenced		 2
#define PG_uptodate		 3

#define PG_dirty	 	 4
#define PG_lru			 5
#define PG_active		 6
#define PG_slab			 7	/* slab debug (Suparna wants this) */

#define PG_checked		 8	/* kill me in 2.5.<early>. */
#define PG_arch_1		 9
#define PG_reserved		10
#define PG_private		11	/* Has something at ->private */

#define PG_writeback		12	/* Page is under writeback */
#define PG_nosave		13	/* Used for system suspend/resume */
#define PG_compound		14	/* Part of a compound page */
#define PG_swapcache		15	/* Swap page: swp_entry_t in private */

#define PG_mappedtodisk		16	/* Has blocks allocated on-disk */
#define PG_reclaim		17	/* To be reclaimed asap */
#define PG_nosave_free		18	/* Free, should not be written */
#define PG_uncached		19	/* Page has been mapped as uncached */

#define ZONETABLE_PGSHIFT		(32 - ZONES_SHIFT)
#define ZONETABLE_MASK		((1UL << ZONES_SHIFT) - 1)

extern struct zone *zone_table[MAX_NR_ZONES];
static inline struct zone *page_zone(struct page *page) {
  return zone_table[(page->flags >> ZONETABLE_PGSHIFT) & ZONETABLE_MASK];
}

struct vm_area_struct{
  struct mm_struct *vm_mm;
  unsigned long vm_start;
  unsigned long vm_end;
  struct vm_area_struct *vm_next;
  unsigned long vm_flags;
  struct rb_node vm_rb;
};
#endif
