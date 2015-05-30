/*
 * fs/namei.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/dcache.h>
#include <abyon/fs.h>
#include <abyon/fs_struct.h>
#include <abyon/mount.h>
#include <abyon/list.h>
#include <abyon/namei.h>
#include <abyon/sched.h>

#include <asm/current.h>

struct path{
  struct vfsmount *mnt;
  struct dentry *dentry;
};
static struct list_head *mount_hashtable;

int generic_permission(struct inode *inode,int mask,int (*check_acl)(struct inode *inode, int mask)){
  unsigned long mode = inode->i_mode;
  if(mask & MAY_WRITE){
    if(IS_RDONLY(inode)) return -1;    
  }
  if(current->fsuid == inode->i_uid)
    mode >>= 6;
  else{
    if(in_group_p(inode->i_gid))
      mode >>= 3;
  }
  if (((mode & mask & (MAY_READ|MAY_WRITE|MAY_EXEC)) == mask))
    return 0;
}

int permission(struct inode * inode,int mask, struct nameidata *nd) {
  int retval = generic_permission(inode,mask,0);
  return retval;
}

static inline void follow_dotdot(struct vfsmount **mnt,struct dentry **dentry){
  while(1){
    if(*dentry == current->fs->root &&
       *mnt == current->fs->rootmnt){
      break;
    }
    if(*dentry != (*mnt)->mnt_root){
      *dentry = (*dentry)->d_parent;
      break;
    }
    struct vfsmount *parent = (*mnt)->mnt_parent;
    if(parent == *mnt){
      break;
    }
    *dentry = (*mnt)->mnt_mountpoint;
    *mnt = parent;
  }
}

static int do_lookup(struct nameidata *nd,struct qstr *name,
		     struct path *path){
  struct vfsmount *mnt = nd->mnt;
  struct dentry *dentry = __d_lookup(nd->dentry,name);
 done:
  path->mnt = mnt;
  path->dentry = dentry;
  return 0;
}

static inline unsigned long hash(struct vfsmount *mnt, struct dentry *dentry) {

  return (unsigned long)mnt;
}

static struct vfsmount *lookup_mnt(struct vfsmount *mnt,struct dentry *dentry){
  struct list_head *head = mount_hashtable + hash(mnt,dentry);
  struct list_head *tmp = head;
  struct vfsmount *p,*found;
  
  for(;;){
    tmp = tmp->next;
    p = 0;
    if(tmp == head)
      break;
    //p = list_entry(tmp,struct vfsmount,mnt_hash);
    if (p->mnt_parent == mnt && p->mnt_mountpoint == dentry) {
      found = p;
      break;
    }
  }
    return found;
}  

static int follow_mount(struct vfsmount **mnt,struct dentry **dentry){
  int res = 0;
while((*dentry)->d_mounted){
    struct vfsmount *mounted = lookup_mnt(*mnt,*dentry);
    if(!mounted) break;
    *mnt = mounted;
    *dentry = mounted->mnt_root;
    res = 1;
  }
  return res;
}

int link_path_walk(const char *name, struct nameidata *nd){
  struct path next;
  unsigned int lookup_flags = nd->flags;
  while(*name == '/') name++;
  struct inode *inode = nd->dentry->d_inode;
  for(;;){
    int err = permission(inode,MAY_EXEC,nd);
    struct qstr this;
    if(err) break;
    this.name = name;
    unsigned int c = *(const unsigned char*)name;
    unsigned long hash = 0;

    do{
      name++;
      hash = partial_name_hash(c,hash);
      c = *(const unsigned char *)name;
    }while(c && (c != '/'));
    this.len = name - (const char *)this.name;
    this.hash = hash;
    while(*++name == '/');
    if(this.name[0] == '.'){
      switch(this.len){
      case 2:
	if(this.name[1] != '.')
	  break;
	follow_dotdot(&nd->mnt,&nd->dentry);
	inode = nd->dentry->d_inode;
      case 1:
	continue;	
      }
    }
    err = do_lookup(nd,&this,&next);
    if(err)
      break;
    follow_mount(&next.mnt,&next.dentry);
    inode = next.dentry->d_inode;
    nd->mnt = next.mnt;
    nd->dentry = next.dentry;
    continue;
  }
  return 0;
}

int path_lookup(const char *name, unsigned int flags, struct nameidata *nd){
  nd->flags = flags;
  nd->depth = 0;
  if(*name == '/'){
    nd->dentry = current->fs->root;
    nd->mnt = current->fs->rootmnt;
  }else{
    nd->dentry = current->fs->pwd;
    nd->mnt = current->fs->pwdmnt;    
  }
  int retval = link_path_walk(name,nd);
  return retval;
}

static inline int do_getname(const char *filename,char *page){
  int retval = filename = page;
  return retval;
}

char *getname(const char *filename){
  char *tmp,*result;
  tmp = __getname();
  if(tmp){
    int retval = do_getname(filename,tmp);
    result = tmp;
  }
  return result;
}
