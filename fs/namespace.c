/*
 * fs/namespace.c
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

typedef struct kmem_cache kmem_cache_t;

static kmem_cache_t *mnt_cache;

void free_vfsmnt(struct vfsmount *mnt){
  kfree(mnt->mnt_devname);
  kmem_cache_free(mnt_cache,mnt);
}

void _mntput(struct vfsmount *mnt){
  struct super_block *sb = mnt->mnt_sb;
  dput(mnt->mnt_root);
  free_vfsmnt(mnt);
  
}

static inline void mntput(struct vfsmount *mnt){
  if(mnt){
    if(--(mnt->mnt_count) <= 0) _mntput(mnt);
  }  
}

void set_fs_pwd(struct fs_struct *fs, struct vfsmount *mnt,struct dentry *dentry){
  struct dentry *old_pwd = fs->pwd;
  struct vfsmount *old_pwdmnt = fs->pwdmnt;
  fs->pwdmnt = mnt;
  fs->pwd = dentry;
  if(old_pwd){
    dput(old_pwd);
    mntput(old_pwdmnt);
  }

}

void set_fs_root(struct fs_struct *fs, struct vfsmount *mnt,struct dentry *dentry){
  struct dentry *old_root = fs->root;
  struct vfsmount *old_rootmnt = fs->rootmnt;
  fs->rootmnt = mnt;
  fs->root = dentry;
  if(old_root){
    dput(old_root);
    mntput(old_rootmnt);
  }
}

static void init_mount_tree(void){
  struct vfsmount *mnt = do_kern_mount("rootfs",0,"rootfs",0);
  struct namespace *namespace = kmalloc(sizeof(struct namespace),0);
  namespace->count = 1;
  INIT_LIST_HEAD(&namespace->list);
  list_add(&mnt->mnt_list,&namespace->list);
  namespace->root = mnt;
  init_task.namespace = namespace;
  struct task_struct *g,*p;
  for(p = &init_task; (p = p->next_task) != &init_task; ){
    get_namespace(namespace);
    p->namespace = namespace;
  }
  set_fs_pwd(current->fs,namespace->root,namespace->root->mnt_root);
  set_fs_root(current->fs,namespace->root,namespace->root->mnt_root);
}

struct vfsmount *alloc_vfsmnt(const char *name){
  struct vfsmount *mnt = kmem_cache_alloc(mnt_cache,0);
  if(mnt){
    INIT_LIST_HEAD(&mnt->mnt_mounts);
    if(name){
      int size = strlen(name) + 1;
      char *newname = kmalloc(size,0);
      if(newname){
	memcpy(newname,name,size);
	mnt->mnt_devname = newname;
      }
    }
  }
  return mnt;
}

void mnt_init(unsigned long mempages){
  mnt_cache = kmem_cache_create("mnt_cache",sizeof(struct vfsmount),
				0,0,0,0);
  sysfs_init();
  init_rootfs();
  init_mount_tree();
}
