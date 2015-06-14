/*
 * fs/open.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/binfmts.h>
#include <abyon/dcache.h>
#include <abyon/fs.h>
#include <abyon/namei.h>
#include <abyon/ptrace.h>

extern kmem_cache_t *filp_cachep;
struct file *dentry_open(struct dentry *dentry, struct vfsmount *mnt, int flags) {
  struct file *f = kmem_cache_alloc(filp_cachep,0);
  f->f_flags = flags;
  struct inode *inode = dentry->d_inode;
  f->f_dentry = dentry;
  f->f_vfsmnt = mnt;
  f->f_pos = 0;
  f->f_op = inode->i_fop;
  //file_move(f,&inode->i_sb->s_files);
  return f;
}
