/*
 * include/abyon/dcache.h
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/fs.h>
#include <abyon/list.h>

#define DNAME_INLINE_LEN_MIN 36

static inline unsigned long partial_name_hash(unsigned long c, unsigned long prevhash) {

return (prevhash + (c << 4) + (c >> 4)) * 11;

}
struct qstr{
unsigned int hash;
unsigned int len;
const unsigned char *name;
};

struct dentry{
unsigned int d_count;
struct qstr d_name;
struct dentry *d_parent;
struct list_head d_lru;
struct inode *d_inode;
unsigned char d_iname[DNAME_INLINE_LEN_MIN];
int d_mounted;
struct hlist_node d_hash;
};
