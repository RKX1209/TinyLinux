/*
 * include/abyon/fs_struct.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef _ABYON_FS_STRUCT_H
#define _ABYON_FS_STRUCT_H
struct fs_struct{
  int count;
  struct dentry *root, *pwd;
  struct vfsmount *rootmnt, *pwdmnt;
};
#endif
