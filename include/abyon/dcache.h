/*
 * include/abyon/dcache.h
 * Copyright (C) <2015>  <@RKX1209>
 */
struct dentry{
  unsigned int d_count;
  struct list_head d_lru;
};
