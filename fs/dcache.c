/*
 * fs/dcache.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/bootmem.h>
#include <abyon/list.h>

static unsigned int d_hash_mask;
static unsigned int d_hash_shift;
static struct hlist_head *dentry_hashtable;
static unsigned long dhash_entries;

extern void inode_init_early(void);
extern int printk(const char *fmt, ...);

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

void vfs_caches_init(unsigned long mempages){
  
}
