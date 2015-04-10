/*
 * mm/slab.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <asm/bitops.h>
#include <asm/page.h>
#include <asm/processor.h>
#include <asm/semaphore.h>

#include <abyon/gfp.h>
#include <abyon/kernel.h>
#include <abyon/mm.h>
#include <abyon/page-flags.h>
#include <abyon/slab.h>

#define	BYTES_PER_WORD		sizeof(void *)
#define BUFCTL_END	(((unsigned int)(~0U))-0)
#define BOOT_CPUCACHE_ENTRIES	1

#define SET_PAGE_CACHE(pg,x) ((pg)->lru.next = (struct list_head*)(x))
#define SET_PAGE_SLAB(pg,x) ((pg)->lru.prev = (struct list_head*)(x))

struct array_cache{
  unsigned int avail;
  unsigned int limit;
  unsigned int batchcount;
  unsigned int touched;
};

struct arraycache_init{
  struct array_cache cache;
  void *entries[BOOT_CPUCACHE_ENTRIES];
};

struct kmem_list3{
  struct list_head slabs_partial;
  struct list_head slabs_full;
  struct list_head slabs_free;
  unsigned long free_objects;
  int free_touched;  
};

struct kmem_cache{
  struct array_cache *array[1];
  unsigned int batchcount;
  unsigned int limit;
  struct kmem_list3 lists;
  unsigned int objsize;
  unsigned int flags;
  unsigned int num;
  unsigned int colour_off;
  unsigned int gfporder;
  unsigned int gfpflags;
  unsigned int colour;
  unsigned int colour_next;
  unsigned int slab_size;
  const char *name;
  struct list_head next;
  void *ctor;
  void *dtor;
};
typedef struct kmem_cache kmem_cache_t;

struct slab{
  struct list_head list;
  unsigned long colouroff;
  void *s_mem;
  unsigned int inuse;
  unsigned int free;
};

static struct semaphore	cache_chain_sem;
static struct list_head cache_chain;

static struct arraycache_init initarray_cache = { {0,BOOT_CPUCACHE_ENTRIES,1,0} };
static struct arraycache_init initarray_generic = { {0,BOOT_CPUCACHE_ENTRIES,1,0} };

static kmem_cache_t cache_cache = {
  .batchcount	= 1,
  .limit	= BOOT_CPUCACHE_ENTRIES,
  .objsize	= sizeof(kmem_cache_t),
  .flags	= SLAB_NO_REAP,
  .name		= "kmem_cache",
};

struct cache_names{
  char *name;
  char *name_dma;
};

struct cache_sizes malloc_sizes[] = {
#define SIZE(x) { .cs_size = (x) },
  SIZE(256)
  SIZE(512)
  SIZE(1024)
  SIZE(2048)

};

static struct cache_names cache_names[] = {
#define CACHE(x) { .name = "size-" #x, .name_dma = "size-" #x "(DMA)" },
  CACHE(256)
  CACHE(512)
  CACHE(1024)
  CACHE(2048)
};

/* Alloc slab in cachep */
static struct slab* alloc_slabmgmt(kmem_cache_t * cachep,void *objp,
				   int colour_off,int local_flags){
  /* OFF_SLAB is always false */
  struct slab *slabp = objp + colour_off;
  colour_off += cachep->slab_size;
  slabp->inuse = 0;
  slabp->colouroff = colour_off;
  slabp->s_mem = objp + colour_off;
  return slabp;
}

/* Alloc objects in a slab */
static void *kmem_getpages(kmem_cache_t *cachep, int flags, int nodeid){
  flags |= cachep->gfpflags;
  struct page *page = alloc_pages_node(nodeid,flags,cachep->gfporder);
  if(!page) return 0;
  int i = (1 << cachep->gfporder);
  void *addr;// = page_to_pfn(page) << PAGE_SHIFT;
  while(i--){
    SetPageSlab(page);
    page++;
  }
  return addr;
}

/* Just set attribute to slabp, NOT alloc */
static void set_slab_attr(kmem_cache_t *cachep,struct slab *slabp,void *objp){
  int i = 1 << cachep->gfporder;
  struct page *page;// = virt_to_page(objp);
  do{
    SET_PAGE_CACHE(page,cachep);
    SET_PAGE_SLAB(page,slabp);
    page++;
  }while(--i);
}

/* Alloc cache */
static int cache_grow (kmem_cache_t * cachep, int flags, int nodeid){
  struct slab *slabp;
  void *objp;
  int offset = cachep->colour_next;
  cachep->colour_next++;
  if(cachep->colour_next >= cachep->colour) cachep->colour_next = 0;
  offset *= cachep->colour_off;
  int local_flags;
  if(!(objp = kmem_getpages(cachep,flags,nodeid))) goto failed;
  if(!(slabp = alloc_slabmgmt(cachep,objp,offset,local_flags))) goto failed;
  set_slab_attr(cachep,slabp,objp);
  list_add_tail(&slabp->list,&(cachep->lists.slabs_free));
  cachep->lists.free_objects += cachep->num;
  return 1;
 failed:
  return 0;
}

/* Get free space(slab) from cachep */
static void *cache_alloc_refill(kmem_cache_t* cachep, int flags){
  struct array_cache *ac = cachep->array[0];
  struct kmem_list3 *l3 = &cachep->lists;
  int batchcount = ac->batchcount;
  while(batchcount > 0){
    struct list_head *entry = l3->slabs_partial.next;
    if(entry == &l3->slabs_partial){
      l3->free_touched - 1;
      entry = l3->slabs_free.next;
      if(entry == &l3->slabs_free)
	goto must_grow;
    }
    struct slab *slabp = list_entry(entry,struct slab,list);
    while(slabp->inuse < cachep->num && batchcount--){
      //(void**)(ac + 1)[ac->avail++] = slabp->s_mem + slabp->free * cachep->objsize;
      slabp->inuse++;
      unsigned long next;// = (unsigned long*)(slabp + 1)[slabp->free];
      slabp->free = next;
      list_del(&slabp->list);
      if(slabp->free == BUFCTL_END)
	list_add(&slabp->list,&l3->slabs_full);
      else
	list_add(&slabp->list,&l3->slabs_partial);
    }
  must_grow:
    l3->free_objects -= ac->avail;
    if(!ac->avail){
      int x = cache_grow(cachep,flags,-1);
      ac = cachep->array[0];
    }
    ac->touched = 1;
  }
  return ((void**)(ac + 1))[--ac->avail];
}

/* Get objects from cachep */
static inline void *__cache_alloc(kmem_cache_t *cachep,int flags){
  unsigned long save_flags;
  void *objp;
  local_irq_save(save_flags);
  struct array_cache *ac = cachep->array[0];
  if(ac->avail){ /* Target exist in local cache */
    ac->touched = 1;
    objp = ((void**)(ac + 1))[--ac->avail];    
  }else{ /* Not exist, should use slab */
    objp = cache_alloc_refill(cachep,flags);
  }
  
  return objp;
}

/* Get objects from cachep */
void *kmem_cache_alloc(kmem_cache_t *cachep,int flags){
  return __cache_alloc(cachep,flags);
}

/* Create kmem_cache_t */
kmem_cache_t *kmem_cache_create(const char *name,unsigned long size,unsigned long align,
				unsigned long flags,
				void (*ctor)(void*,kmem_cache_t *,unsigned long),
				void (*dtor)(void*,kmem_cache_t *,unsigned long)){
  kmem_cache_t *cachep = 0;
  align = BYTES_PER_WORD;
  cachep = (kmem_cache_t *) kmem_cache_alloc(&cache_cache,0);
  //[TODO] define SLAB_KERNEL
  INIT_LIST_HEAD(&cachep->lists.slabs_full);
  INIT_LIST_HEAD(&cachep->lists.slabs_partial);
  INIT_LIST_HEAD(&cachep->lists.slabs_free);
  cachep->ctor = ctor;
  cachep->dtor = dtor;
  cachep->name = name;

  list_add(&cachep->next,&cache_chain);
  return cachep;
}

void kmem_cache_init(void){
  unsigned long left_over = 128;
  struct cache_sizes *sizes;
  struct cache_names *names;

  init_MUTEX(&cache_chain_sem);
  INIT_LIST_HEAD(&cache_chain);
  list_add(&cache_cache.next,&cache_chain);
  cache_cache.colour_off = cache_line_size();
  cache_cache.array[0] = &initarray_cache.cache;
  cache_cache.objsize = ALIGN(cache_cache.objsize,cache_line_size());
  cache_cache.colour = left_over / cache_cache.colour_off;
  cache_cache.colour_next = 0;
  cache_cache.slab_size = ALIGN(cache_cache.num * sizeof(unsigned int) + sizeof(struct slab),
				cache_line_size());
  sizes = malloc_sizes;
  names = cache_names;
  while(sizes->cs_size){
    sizes->cs_cachep = kmem_cache_create(names->name,sizes->cs_size,
					 0,0,0,0);
    sizes++;
  }
}
