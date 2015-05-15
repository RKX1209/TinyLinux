/*
 * fs/ramfs/inode.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/fs.h>

static struct super_block *rootfs_get_sb(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
  return 0;
}
static struct file_system_type rootfs_fs_type = {
  .name = "rootfs",
  .get_sb = rootfs_get_sb,  
};

int init_rootfs(void){
  return register_filesystem(&rootfs_fs_type);
}
