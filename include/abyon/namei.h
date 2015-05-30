/*
 * include/abyon/namei.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ABYON_NAMEI_H
#define _ABYON_NAMEI_H
#include <abyon/fs.h>

struct nameidata{
  struct dentry *dentry;
  struct vfsmount *mnt;
  unsigned int flags;
  unsigned int depth;
};
#endif
