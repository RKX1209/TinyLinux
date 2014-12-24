/*
 * mm/page_alloc.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/kernel.h>
#include <abyon/list.h>
#include <abyon/mm.h>

#include <asm/bitops.h>

struct pglist_data *pgdat_list;
struct zone *zone_table[MAX_NR_ZONES];

unsigned long nr_free_pages(void){
  unsigned int sum = 0;
  struct zone *zone;
  for(zone = pgdat_list->node_zones; zone; zone = next_zone(zone)){
    sum += zone->free_pages;
  }
  return sum;
}

static inline int page_is_buddy(struct page *page, int order){
  if(test_bit(PG_private, &page->flags) &&
     page->private == order &&
     page->_count == 0)
    return 1;
  return 0;
}
  
static inline void __free_pages_bulk (struct page *page,
				      struct zone *zone, unsigned int order){
  unsigned long page_idx = page_to_pfn(page) & ((1 << MAX_ORDER) - 1);
  struct page *base = zone->zone_mem_map;
  int order_size = 1 << order;
  zone->free_pages += order_size;
  while(order < MAX_ORDER - 1){
    unsigned long buddy_idx = page_idx ^ (1 << order);
    struct page *buddy = base + buddy_idx;
    if(!page_is_buddy(buddy,order)) break;
    list_del(&buddy->lru);
    zone->free_area[order].nr_free--;
    __clear_bit(PG_private,&buddy->flags);
    buddy->private = 0;
    page_idx &= buddy_idx;
    order++;
  }
  page->private = order;
  list_add(&page->lru,&zone->free_area[order].free_list);
  zone->free_area[order].nr_free++;
}

static int free_pages_bulk(struct zone *zone, int count,
			   struct list_head *list, unsigned int order){
  int ret = 0;
  zone->all_unreclaimable = 0;
  zone->pages_scanned = 0;
  while(!list_empty(list) && count--){
    struct page *page = list_entry(list->prev,struct page,lru);
    list_del(&page->lru);
    __free_pages_bulk(page, zone, order);
    ret++;
  }
  return ret;
}
void __free_pages_ok(struct page *page, unsigned int order){
  LIST_HEAD(list);
  list_add(&page->lru,&list);
  free_pages_bulk(page_zone(page),1,&list,order);
}

fastcall void __free_pages(struct page *page,unsigned int order){
  if(page->_count == 0){
    if(order == 0);
    else __free_pages_ok(page,order);
  }
}
