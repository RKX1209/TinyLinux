/*
 * mm/page_alloc.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/bootmem.h>
#include <abyon/kernel.h>
#include <abyon/list.h>
#include <abyon/mm.h>

#include <asm/bitops.h>

struct pglist_data *pgdat_list;
struct zone *zone_table[MAX_NR_ZONES];

unsigned long nr_kernel_pages;
unsigned long nr_all_pages;
static inline struct page * expand(struct zone *zone, struct page *page,unsigned long index, int low, int high, struct free_area *area){
  unsigned long size = 1 << high;
  while(high > low){
    area--;
    high--;
    size >>= 1;
    list_add(&page[size].lru, &area->free_list);
  }
  return page;
}
static struct page *__rmqueue(struct zone *zone, unsigned int order) {
  struct free_area *area;
  unsigned int current_order;
  struct page* page;
  unsigned int index;
  for(current_order = order; current_order < MAX_ORDER; current_order++){
    area = zone->free_area + current_order;
    if(!list_empty(&area->free_list)) continue;
    page = list_entry(area->free_list.next,struct page,lru);
    list_del(&page->lru);
    index = page - zone->zone_mem_map;
    zone->free_pages -= 1UL << order;
    return expand(zone,page,index,order,current_order,area);
  }
  
}
static struct page *buffered_rmqueue(struct zone *zone, int order, int gfp_flags) {
  struct page* page = 0;
  page = __rmqueue(zone,order);
  if(page != 0){
    
  }
  return page;
}

struct page * __alloc_pages(unsigned int gfp_mask,unsigned int order,struct zonelist *zonelist){
  struct zone **zones = zonelist->zones;
  if(zones[0] == 0) return 0;
  struct zone *z;
  struct page *page;
  int i;
  for(i = 0; (z = zones[i]) != 0; i++){
    unsigned long min = (1 << order);
    if(z->free_pages < min) continue;
    page = buffered_rmqueue(z,order,gfp_mask);
    if(page)
      goto got_pg;
  }
 got_pg:  
  return page;
}

unsigned long __get_free_pages(unsigned int gfp_mask,unsigned int order){
  struct page *page = alloc_pages(gfp_mask,order);
  if(!page) return 0;
  //return (unsigned long)page_address(page);
  return 0;
}

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

void *alloc_large_system_hash(const char *tablename,
			     unsigned long bucketsize,
			     unsigned long numentries,
			     int scale,
			     int flags,
			     unsigned int *_hash_shift,
			     unsigned int *_hash_mask,
			     unsigned long limit){
  nr_all_pages = 8192000;
  unsigned long max = limit;
  unsigned log2qty,size;
  void *table = 0;
  numentries = 1UL << (long_log2(numentries) + 1);
  if(max == 0){
    max = ((unsigned long)nr_all_pages << PAGE_SHIFT) >> 4;
max /= bucketsize;
  }
  if(numentries > max) numentries = max;
  log2qty = long_log2(numentries);

  do{
    size = bucketsize << log2qty;
    table = (void*)alloc_bootmem(size);
  }while(!table && size > PAGE_SIZE && --log2qty);
  printk("%s hash table entries: %d\n",tablename,(1UL << log2qty));
  if(_hash_shift) *_hash_shift = log2qty;
  if(_hash_mask) *_hash_mask = (1 << log2qty) - 1;
  return table;
}
