/*
 * include/abyon/security.h
 * Copyright (C) <2015>  <@RKX1209>
 */

#ifndef _ABYON_SECURITY_H
#define _ABYON_SECURITY_H
#include <abyon/fs.h>
#include <abyon/binfmts.h>

struct security_operations {
  int (*ptrace) (struct task_struct * parent, struct task_struct * child);
  /* int (*capget) (struct task_struct * target, */
  /* 		 kernel_cap_t * effective, */
  /* 		 kernel_cap_t * inheritable, kernel_cap_t * permitted); */
  /* int (*capset_check) (struct task_struct * target, */
  /* 		       kernel_cap_t * effective, */
  /* 		       kernel_cap_t * inheritable, */
  /* 		       kernel_cap_t * permitted); */
  /* void (*capset_set) (struct task_struct * target, */
  /* 		      kernel_cap_t * effective, */
  /* 		      kernel_cap_t * inheritable, */
  /* 		      kernel_cap_t * permitted); */
  int (*acct) (struct file * file);
  //int (*sysctl) (struct ctl_table * table, int op);
  int (*capable) (struct task_struct * tsk, int cap);
  int (*quotactl) (int cmds, int type, int id, struct super_block * sb);
  int (*quota_on) (struct dentry * dentry);
  int (*syslog) (int type);
  //int (*settime) (struct timespec *ts, struct timezone *tz);
  int (*vm_enough_memory) (long pages);

  int (*bprm_alloc_security) (struct abyon_binprm * bprm);
  void (*bprm_free_security) (struct abyon_binprm * bprm);
  void (*bprm_apply_creds) (struct abyon_binprm * bprm, int unsafe);
  void (*bprm_post_apply_creds) (struct abyon_binprm * bprm);
  int (*bprm_set_security) (struct abyon_binprm * bprm);
  int (*bprm_check_security) (struct abyon_binprm * bprm);
  int (*bprm_secureexec) (struct abyon_binprm * bprm);

  int (*sb_alloc_security) (struct super_block * sb);
  void (*sb_free_security) (struct super_block * sb);
  int (*sb_copy_data)(struct file_system_type *type,
		      void *orig, void *copy);
  int (*sb_kern_mount) (struct super_block *sb, void *data);
  int (*sb_statfs) (struct super_block * sb);
  int (*sb_mount) (char *dev_name, struct nameidata * nd,
		   char *type, unsigned long flags, void *data);
  int (*sb_check_sb) (struct vfsmount * mnt, struct nameidata * nd);
  int (*sb_umount) (struct vfsmount * mnt, int flags);
  void (*sb_umount_close) (struct vfsmount * mnt);
  void (*sb_umount_busy) (struct vfsmount * mnt);
  void (*sb_post_remount) (struct vfsmount * mnt,
			   unsigned long flags, void *data);
  void (*sb_post_mountroot) (void);
  /* void (*sb_post_addmount) (struct vfsmount * mnt, */
  /* 			    struct nameidata * mountpoint_nd); */
  /* int (*sb_pivotroot) (struct nameidata * old_nd, */
  /* 		       struct nameidata * new_nd); */
  /* void (*sb_post_pivotroot) (struct nameidata * old_nd, */
  /* 			     struct nameidata * new_nd); */

  int (*inode_alloc_security) (struct inode *inode);	
  void (*inode_free_security) (struct inode *inode);
  int (*inode_init_security) (struct inode *inode, struct inode *dir,
			      char **name, void **value, unsigned int *len);
  int (*inode_create) (struct inode *dir,
		       struct dentry *dentry, int mode);
  int (*inode_link) (struct dentry *old_dentry,
		     struct inode *dir, struct dentry *new_dentry);
  int (*inode_unlink) (struct inode *dir, struct dentry *dentry);
  int (*inode_symlink) (struct inode *dir,
			struct dentry *dentry, const char *old_name);
  int (*inode_mkdir) (struct inode *dir, struct dentry *dentry, int mode);
  int (*inode_rmdir) (struct inode *dir, struct dentry *dentry);
  /* int (*inode_mknod) (struct inode *dir, struct dentry *dentry, */
  /* 		      int mode, dev_t dev); */
  int (*inode_rename) (struct inode *old_dir, struct dentry *old_dentry,
		       struct inode *new_dir, struct dentry *new_dentry);
  int (*inode_readlink) (struct dentry *dentry);
  /* int (*inode_follow_link) (struct dentry *dentry, struct nameidata *nd); */
  /* int (*inode_permission) (struct inode *inode, int mask, struct nameidata *nd); */
  int (*inode_setattr)	(struct dentry *dentry, struct iattr *attr);
  int (*inode_getattr) (struct vfsmount *mnt, struct dentry *dentry);
  void (*inode_delete) (struct inode *inode);
  int (*inode_setxattr) (struct dentry *dentry, char *name, void *value,
			 unsigned int size, int flags);
  void (*inode_post_setxattr) (struct dentry *dentry, char *name, void *value,
			       unsigned int size, int flags);
  int (*inode_getxattr) (struct dentry *dentry, char *name);
  int (*inode_listxattr) (struct dentry *dentry);
  int (*inode_removexattr) (struct dentry *dentry, char *name);
  int (*inode_getsecurity)(struct inode *inode, const char *name, void *buffer, unsigned int size, int err);
  int (*inode_setsecurity)(struct inode *inode, const char *name, const void *value, unsigned int size, int flags);
  int (*inode_listsecurity)(struct inode *inode, char *buffer, unsigned int buffer_size);

  int (*file_permission) (struct file * file, int mask);
  int (*file_alloc_security) (struct file * file);
  void (*file_free_security) (struct file * file);
  int (*file_ioctl) (struct file * file, unsigned int cmd,
		     unsigned long arg);
  int (*file_mmap) (struct file * file,
		    unsigned long reqprot,
		    unsigned long prot, unsigned long flags);
  int (*file_mprotect) (struct vm_area_struct * vma,
			unsigned long reqprot,
			unsigned long prot);
  int (*file_lock) (struct file * file, unsigned int cmd);
  int (*file_fcntl) (struct file * file, unsigned int cmd,
		     unsigned long arg);
  int (*file_set_fowner) (struct file * file);
  /* int (*file_send_sigiotask) (struct task_struct * tsk, */
  /* 			      struct fown_struct * fown, int sig); */
  int (*file_receive) (struct file * file);

  int (*task_create) (unsigned long clone_flags);
  int (*task_alloc_security) (struct task_struct * p);
  void (*task_free_security) (struct task_struct * p);
  int (*task_setuid) (unsigned int id0, unsigned int id1, unsigned int id2, int flags);
  int (*task_post_setuid) (unsigned int old_ruid /* or fsuid */ ,
			   unsigned int old_euid, unsigned int old_suid, int flags);
  int (*task_setgid) (unsigned int id0, unsigned int id1, unsigned int id2, int flags);
  int (*task_setpgid) (struct task_struct * p, unsigned int pgid);
  int (*task_getpgid) (struct task_struct * p);
  int (*task_getsid) (struct task_struct * p);
  int (*task_setgroups) (struct group_info *group_info);
  int (*task_setnice) (struct task_struct * p, int nice);
  int (*task_setrlimit) (unsigned int resource, struct rlimit * new_rlim);
  int (*task_setscheduler) (struct task_struct * p, int policy,
			    struct sched_param * lp);
  int (*task_getscheduler) (struct task_struct * p);
  int (*task_kill) (struct task_struct * p,
		    struct siginfo * info, int sig);
  int (*task_wait) (struct task_struct * p);
  int (*task_prctl) (int option, unsigned long arg2,
		     unsigned long arg3, unsigned long arg4,
		     unsigned long arg5);
  void (*task_reparent_to_init) (struct task_struct * p);
  void (*task_to_inode)(struct task_struct *p, struct inode *inode);

  /* int (*ipc_permission) (struct kern_ipc_perm * ipcp, short flag); */

  /* int (*msg_msg_alloc_security) (struct msg_msg * msg); */
  /* void (*msg_msg_free_security) (struct msg_msg * msg); */

  /* int (*msg_queue_alloc_security) (struct msg_queue * msq); */
  /* void (*msg_queue_free_security) (struct msg_queue * msq); */
  /* int (*msg_queue_associate) (struct msg_queue * msq, int msqflg); */
  /* int (*msg_queue_msgctl) (struct msg_queue * msq, int cmd); */
  /* int (*msg_queue_msgsnd) (struct msg_queue * msq, */
  /* 			   struct msg_msg * msg, int msqflg); */
  /* int (*msg_queue_msgrcv) (struct msg_queue * msq, */
  /* 			   struct msg_msg * msg, */
  /* 			   struct task_struct * target, */
  /* 			   long type, int mode); */

  /* int (*shm_alloc_security) (struct shmid_kernel * shp); */
  /* void (*shm_free_security) (struct shmid_kernel * shp); */
  /* int (*shm_associate) (struct shmid_kernel * shp, int shmflg); */
  /* int (*shm_shmctl) (struct shmid_kernel * shp, int cmd); */
  /* int (*shm_shmat) (struct shmid_kernel * shp,  */
  /* 		    char __user *shmaddr, int shmflg); */

  /* int (*sem_alloc_security) (struct sem_array * sma); */
  /* void (*sem_free_security) (struct sem_array * sma); */
  /* int (*sem_associate) (struct sem_array * sma, int semflg); */
  /* int (*sem_semctl) (struct sem_array * sma, int cmd); */
  /* int (*sem_semop) (struct sem_array * sma,  */
  /* 		    struct sembuf * sops, unsigned nsops, int alter); */


  /* allow module stacking */
  int (*register_security) (const char *name,
			    struct security_operations *ops);
  int (*unregister_security) (const char *name,
			      struct security_operations *ops);

};
#endif
