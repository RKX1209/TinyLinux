/*
 * include/abyon/gfp.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/mmzone.h>

#define alloc_page(gfp_mask) alloc_pages(gfp_mask,0)

struct page *alloc_pages(unsigned gfp,unsigned order){
  return __alloc_pages(gfp,order,NODE_DATA(0)->node_zonelists + (gfp & GFP_ZONEMASK));
}

static inline struct page *alloc_pages_node(int nid, unsigned int gfp_mask,unsigned int order){
  return __alloc_pages(gfp_mask,order,NODE_DATA(nid)->node_zonelists + (gfp_mask & GFP_ZONEMASK));
}
