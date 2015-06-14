/*
 * fs/dcache.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/bootmem.h>
#include <abyon/dcache.h>
#include <abyon/fs.h>
#include <abyon/kernel.h>
#include <abyon/limits.h>
#include <abyon/list.h>

#define GOLDEN_RATIO_PRIME 0x9e370001UL
static unsigned int d_hash_shift;
static unsigned int d_hash_mask;
#define D_HASHBITS     d_hash_shift
#define D_HASHMASK     d_hash_mask

typedef struct kmem_cache kmem_cache_t;

extern struct files_stat_struct files_stat;
static unsigned int d_hash_mask;
static unsigned int d_hash_shift;
static struct hlist_head *dentry_hashtable;
static unsigned long dhash_entries;

extern void inode_init_early(void);
extern int printk(const char *fmt, ...);

static kmem_cache_t *dentry_cache;
static LIST_HEAD(dentry_unused);

/* SLAB cache for __getname() consumers */
kmem_cache_t *names_cachep;
/* SLAB cache for file structures */
kmem_cache_t *filp_cachep;
/* SLAB cache for inode */
kmem_cache_t *inode_cachep;

void dcache_init_early(void){
  dhash_entries = 1024;
  dentry_hashtable = alloc_large_system_hash("Dentry cache",
					     sizeof(struct hlist_head),
					     dhash_entries,
					     13,
					     HASH_EARLY,
					     &d_hash_shift,
					     &d_hash_mask,
					     0);
  int loop;  
  for(loop = 0; loop < (1 << d_hash_shift); loop++)
    INIT_HLIST_HEAD(&dentry_hashtable[loop]);
}

void vfs_caches_init_early(void){
  printk("vfs_init_early...");
  dcache_init_early();
  inode_init_early();
  printk("vfs_init_early... [OK]");
}

static void dcache_init(unsigned long mempages){
  dentry_cache = kmem_cache_create("dentry_cache",
				   sizeof(struct dentry),
				   0,0,0,0);
}

static void inode_init(unsigned long mempages){
  inode_cachep = kmem_cache_create("inode_cache",
				   sizeof(struct inode),
				   0,0,0,0);
}

void files_init(unsigned long mempages){
  int n = (mempages * (PAGE_SIZE / 1024)) / 10;
  files_stat.max_files = n;
  if(files_stat.max_files < NR_FILE)
    files_stat.max_files = NR_FILE;  
}

extern void bdev_cache_init(void);
extern void cdev_cache_init(void);

void vfs_caches_init(unsigned long mempages){
  unsigned long reserve = min((mempages - nr_free_pages()) * 3 / 2,
			      mempages - 1);
  mempages -= reserve;
  names_cachep = kmem_cache_create("names_cache",PATH_MAX,0,
				   0,0,0);
  filp_cachep = kmem_cache_create("filp",sizeof(struct file),0,
				  0,0,0);
  dcache_init(mempages);
  inode_init(mempages);
  files_init(mempages);
  mnt_init(mempages);
  bdev_cache_init();
  chrdev_init();
}

void dput(struct dentry *dentry){
  if(!dentry) return;
  dentry->d_count--;
  /* It has been referenced by some process yet */
  if(dentry->d_count){
    return;
  }
  if(list_empty(&dentry->d_lru)){
    list_add(&dentry->d_lru,&dentry_unused);
  }
  return;
}

static inline struct hlist_head *d_hash(struct dentry *parent,unsigned long hash){
  hash += ((unsigned long) parent ^ GOLDEN_RATIO_PRIME);
  hash = hash ^ ((hash ^ GOLDEN_RATIO_PRIME) >> D_HASHBITS);
  return dentry_hashtable + (hash & D_HASHMASK);
}

struct dentry * __d_lookup(struct dentry *parent,struct qstr *name){
  unsigned int len = name->len;
  unsigned int hash = name->hash;
  const unsigned char *str = name->name;
  struct hlist_head *head = d_hash(parent,hash);
  struct dentry *found = 0;
  struct hlist_node *node;
  for(node = head->first; node; node = node->next){
    struct dentry *dentry = list_entry(node,struct dentry,d_hash);
    struct qstr *qstr;
    if (dentry->d_name.hash != hash)
      continue;
    if (dentry->d_parent != parent)
      continue;
    qstr = &dentry->d_name;
    found = dentry;
  }
  return found;
}

struct dentry * d_lookup(struct dentry * parent, struct qstr * name) {
  struct dentry *dentry = 0;
  return dentry;
}
