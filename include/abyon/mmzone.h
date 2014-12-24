/*
 * include/abyon/mmzone.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef _ABYON_MMZONE_H
#define _ABYON_MMZONE_H


#define MAX_ORDER 11

#define ZONE_DMA		0
#define ZONE_NORMAL		1
#define ZONE_HIGHMEM		2

#define MAX_NR_ZONES		3	/* Sync this with ZONES_SHIFT */
#define ZONES_SHIFT		2	/* ceil(log2(MAX_NR_ZONES)) */

struct bootmem_data;

struct free_area {
  struct list_head	free_list;
  unsigned long		nr_free;
};

struct zone{
  unsigned long free_pages;
  struct pglist_data *zone_pgdat;
  unsigned long pages_scanned;
  int all_unreclaimable;
  struct free_area free_area[MAX_ORDER];
  struct page *zone_mem_map;
  unsigned long zone_start_pfn;
};

typedef struct pglist_data{
  struct zone node_zones[MAX_NR_ZONES];
  int nr_zones;
  struct page *node_mem_map;
  int node_id;
  struct bootmem_data *bdata; /* Boot allocator*/
  unsigned long node_start_pfn;
  struct pglist_data *pgdat_next;
}pg_data_t;
extern struct pglist_data *pgdat_list;

struct pglist_data node_data[1]; /* numbef of NUMA node is 'one' */
#define NODE_DATA(nid)	(&node_data[nid])

static inline struct zone *next_zone(struct zone *zone){
  pg_data_t *pgdat = zone->zone_pgdat;
  if(zone - pgdat->node_zones < MAX_NR_ZONES - 1) zone++;
  return zone;
}


#define node_localnr(pfn, nid) ((pfn) - node_data[nid].node_start_pfn)
#define node_mem_map(nid) (NODE_DATA(nid)->node_mem_map)

#define page_to_pfn(pg)({						\
      struct page *__page = pg;						\
      struct zone *__zone = page_zone(__page);				\
      (unsigned long)(__page - __zone->zone_mem_map) + __zone->zone_start_pfn; \
    })

#define pfn_to_page(pfn)({						\
			  unsigned long __pfn = (pfn);			\
			  int __node = 0;				\
			  &node_mem_map(__node)[node_localnr(__pfn,__node)]; \
    })

  
#endif
