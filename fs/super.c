/*
 * fs/super.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/fs.h>
#include <abyon/mount.h>

struct file_system_type *file_systems;

struct file_system_type *get_fs_type(const char *name){
  struct file_system_type *fs;
  //fs = *(find_filesystem(name));
  return fs;
}
struct vfsmount *do_kern_mount(const char *fstype,int flags,
			       const char *name,void *data){
  struct file_system_type *type = get_fs_type(fstype);
  struct vfsmount *mnt = alloc_vfsmnt(name);
  struct super_block *sb = type->get_sb(type,name,data);
  mnt->mnt_sb = sb;
  mnt->mnt_root = sb->s_root;
  
}

struct vfsmount *kern_mount(struct file_system_type *type){
  return do_kern_mount(type->name,0,type->name,0);
}
