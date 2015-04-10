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

#define SetPageSlab(page) __set_bit(PG_slab,(page)->flags)
