/*
 * include/abyon/mount.h
 * Copyright (C) <2015>  <@RKX1209>
 */

#ifndef _ABYON_MOUNT_H
#define _ABYON_MOUNT_H

#include <abyon/list.h>

struct vfsmount{
  struct vfsmount *mnt_parent;
  struct dentry *mnt_mountpoint;
  struct dentry *mnt_root;
  struct list_head mnt_mounts;
  struct list_head mnt_child;
  struct super_block *mnt_sb;
  char *mnt_devname;
  struct list_head mnt_list;
  unsigned long mnt_count;
};

struct list_head mnt_hash;
#endif
