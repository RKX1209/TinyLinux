/*
 * include/abyon/slab.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef _ABYON_SLAB_H
#define _ABYON_SLAB_H

#define	SLAB_DEBUG_FREE		0x00000100UL	/* Peform (expensive) checks on free */
#define	SLAB_DEBUG_INITIAL	0x00000200UL	/* Call constructor (as verifier) */
#define	SLAB_RED_ZONE		0x00000400UL	/* Red zone objs in a cache */
#define	SLAB_POISON		0x00000800UL	/* Poison objects */
#define	SLAB_NO_REAP		0x00001000UL	/* never reap from the cache */
#define	SLAB_HWCACHE_ALIGN	0x00002000UL	/* align objs on a h/w cache lines */
#define SLAB_CACHE_DMA		0x00004000UL	/* use GFP_DMA memory */
#define SLAB_MUST_HWCACHE_ALIGN	0x00008000UL	/* force alignment */
#define SLAB_STORE_USER		0x00010000UL	/* store the last owner for bug hunting */
#define SLAB_RECLAIM_ACCOUNT	0x00020000UL	/* track pages allocated to indicate
						   what is reclaimable later*/
#define SLAB_PANIC		0x00040000UL	/* panic if kmem_cache_create() fails */
#define SLAB_DESTROY_BY_RCU	0x00080000UL	/* defer freeing pages to RCU */
struct kmem_cache;
struct cache_sizes{
  unsigned long cs_size;
  struct kmem_cache *cs_cachep;
  struct kmem_cache *cs_dmacachep;
};

#endif
