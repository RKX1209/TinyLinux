/*
 * include/abyon/fs.h
 * Copyright (C) <2015>  <@RKX1209>
 */
#ifndef _ABYON_FS_H
#define _ABYON_FS_H

#include <abyon/list.h>
#include <abyon/slab.h>

#define NR_FILE 8192

#define MAY_EXEC 1
#define MAY_WRITE 2
#define MAY_READ 4
#define MAY_APPEND 8

#define MS_RDONLY	 1	/* Mount read-only */
#define MS_NOSUID	 2	/* Ignore suid and sgid bits */
#define MS_NODEV	 4	/* Disallow access to device special files */
#define MS_NOEXEC	 8	/* Disallow program execution */
#define MS_SYNCHRONOUS	16	/* Writes are synced at once */
#define MS_REMOUNT	32	/* Alter flags of a mounted FS */
#define MS_MANDLOCK	64	/* Allow mandatory locks on an FS */
#define MS_DIRSYNC	128	/* Directory modifications are synchronous */
#define MS_NOATIME	1024	/* Do not update access times. */
#define MS_NODIRATIME	2048	/* Do not update directory access times */
#define MS_BIND		4096
#define MS_MOVE		8192
#define MS_REC		16384
#define MS_VERBOSE	32768
#define MS_UNBINDABLE	(1<<17)	/* change to unbindable */
#define MS_PRIVATE	(1<<18)	/* change to private */
#define MS_SLAVE	(1<<19)	/* change to slave */
#define MS_SHARED	(1<<20)	/* change to shared */
#define MS_POSIXACL	(1<<16)	/* VFS does not apply the umask */
#define MS_ACTIVE	(1<<30)
#define MS_NOUSER	(1<<31)

extern kmem_cache_t* names_cachep;
#define __getname() kmem_cache_alloc(names_cachep,0)

#define IS_RDONLY(inode) ( (inode)->i_sb->s_flags & MS_RDONLY)

typedef unsigned long sector_t;
struct files_stat_struct{
  int nr_files;
  int nr_free_files;
  int max_files;
};

typedef struct {
  unsigned int written;
  unsigned int count;
  char *buf;  
}read_descriptor_t;
  
struct file_operations{
  unsigned long (*read)(struct file*, char *, unsigned int, unsigned int);
  unsigned long (*write)(struct file*, char *, unsigned int, unsigned int);
};

struct super_block{
  struct dentry *s_root;
  unsigned long s_flags;
  struct list_head s_files;
};

struct file_system_type{
  const char *name;
  int fs_flags;
  struct super_block *(*get_sb)(struct file_system_type *, const char *,void *);
  void (*kill_sb) (struct super_block*);
  
  struct file_system_type *next;
  struct list_head fs_supers;
};

struct file{
  struct dentry *f_dentry;
  struct vfsmount *f_vfsmnt;
  unsigned int f_flags;
  int f_pos;
  struct file_operations *f_op;
  unsigned int f_uid,f_gid;
};

struct address_space_operations{
  int (*readpage)(struct file *, struct page *);
};

struct address_space{
  struct inode *host;
  struct address_space_operations *a_ops;
};

struct inode{
  unsigned long i_mode;
  unsigned long i_uid,i_gid;
  unsigned int i_blkbits;
  struct super_block *i_sb;
  struct fileoperations *i_fop;
  struct address_space *i_mapping;
  unsigned int i_size;
};

struct gendisk{
  int major;
  int minors;
  char disk_name[32];
};
struct block_device{
  struct inode *bd_inode;
  int bd_openers;
  struct gendisk *bd_disk;
};
#endif
