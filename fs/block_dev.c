/*
 * fs/block_dev.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/fs.h>
#include <abyon/fs_struct.h>
#include <abyon/init_task.h>
#include <abyon/list.h>
#include <abyon/mount.h>
#include <abyon/namespace.h>
#include <abyon/sched.h>

#include <asm/current.h>
#include <asm/string.h>

struct bdev_inode{
  struct block_device bdev;
  struct inode vfs_inode;
};

static struct kmem_cache * bdev_cachep;
static struct file_system_type bd_type = {
  .name = "bdev",
  /* .get_sb = bd_get_sb, */
  /* .kill_sb = kill_anon_super, */
};

struct super_block *blockdev_superblock;
void bdev_cache_init(void){
  bdev_cachep = kmem_cache_create("bdev_cache",sizeof(struct bdev_inode),0,0,0);
  int err = register_filesystem(&bd_type);
  struct vfsmount *bd_mnt = kern_mount(&bd_type);
  blockdev_superblock = bd_mnt->mnt_sb;
}
