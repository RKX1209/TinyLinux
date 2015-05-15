/*
 * fs/sysfs/mount.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/fs.h>
#include <abyon/mount.h>

struct vfsmount *sysfs_mount;
static struct file_system_type sysfs_fs_type = {
  .name = "sysfs",
  .get_sb = 0,
  .kill_sb = 0,
};
int sysfs_init(void){
  int err = register_filesystem(&sysfs_fs_type);
  if(!err){
    sysfs_mount = kern_mount(&sysfs_fs_type);
  }
  return err;
}
