/*
 * fs/inode.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/bootmem.h>
#include <abyon/list.h>

static unsigned int i_hash_mask;
static unsigned int i_hash_shift;
static struct hlist_head *inode_hashtable;
static unsigned long ihash_entries;

void inode_init_early(void){
  ihash_entries = 1024;
  inode_hashtable = alloc_large_system_hash("Inode-cache",
					    sizeof(struct hlist_head),
					    ihash_entries,
					    14,
					    HASH_EARLY,
					    &i_hash_shift,
					    &i_hash_mask,
					    0);
  int loop;
  for(loop = 0; loop < (1 << i_hash_shift); loop++)
    INIT_HLIST_HEAD(&inode_hashtable[loop]);
}
