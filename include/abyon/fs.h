/*
 * include/abyon/fs.h
 * Copyright (C) <2015>  <@RKX1209>
 */
#ifndef _ABYON_FS_H
#define _ABYON_FS_H
#include <abyon/list.h>

#define NR_FILE 8192

struct files_stat_struct{
  int nr_files;
  int nr_free_files;
  int max_files;
};

struct super_block{
  struct dentry *s_root;
};

struct file_system_type{
  const char *name;
  int fs_flags;
  struct super_block *(*get_sb)(struct file_system_type *, const char *,void *);
  void (*kill_sb) (struct super_block*);
  
  struct file_system_type *next;
  struct list_head fs_supers;
};
struct file{
  
};

struct inode{
  
};

struct block_device{
};
#endif
