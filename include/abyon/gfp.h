/*
 * include/abyon/gfp.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/mmzone.h>

static inline struct page *alloc_pages_node(int nid, unsigned int gfp_mask,unsigned int order){
  return __alloc_pages(gfp_mask,order,NODE_DATA(nid)->node_zonelists + (gfp_mask & GFP_ZONEMASK));
}
