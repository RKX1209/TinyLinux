/*
 * fs/exec.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/binfmts.h>
#include <abyon/dcache.h>
#include <abyon/fs.h>
#include <abyon/namei.h>
#include <abyon/ptrace.h>
#include <abyon/sched.h>

#include <asm/current.h>
#include <asm/processor.h>

static struct abyon_binfmt *formats;

int register_binfmt(struct abyon_binfmt *fmt){
  struct abyon_binfmt ** tmp = &formats;
  while(*tmp){
    tmp = &(*tmp)->next;
  }
  fmt->next = formats;
  formats = fmt;
  return 0;
}

struct file *open_exec(const char *name){
  struct nameidata nd;
  struct file *file;
  int err = path_lookup(name,0,&nd);
  if(!err){
    struct inode *inode = nd.dentry->d_inode;
    int err = permission(inode,MAY_EXEC,&nd);
    if(!err){
      file = dentry_open(nd.dentry,nd.mnt,0);
      return file;
    }
  }
}

static int count(char **argv,int max){
  int i = 0;
  if(argv != 0){
    for(;;){
      char *p;
      if(!p) break;
      argv++;
      if(++i > max) return -1;
    }
  }
  return i;
}

int kernel_read(struct file *file,unsigned long offset,
		char *addr,unsigned long count){
  int result = vfs_read(file,(void*)addr,count,&offset);
}

int prepare_binprm(struct abyon_binprm *bprm){
  struct inode *inode = bprm->file->f_dentry->d_inode;
  int mode = inode->i_mode;
  bprm->e_uid = current->euid;
  bprm->e_gid = current->egid;
  return kernel_read(bprm->file,0,bprm->buf,BINPRM_BUF_SIZE);  
  
}

int search_binary_handler(struct abyon_binprm *bprm,struct pt_regs *regs) {
  struct abyon_binfmt *fmt;
  for(fmt = formats; fmt; fmt = fmt->next){
    int (*fn)(struct abyon_binprm *, struct pt_regs *) = fmt->load_binary;
    if(!fn) continue;
    int retval = fn(bprm,regs);
    if(retval >= 0){
      
      return retval;
    }
  }  
}

int do_execve(char *filename,
	      char *argv,
	      char *envp,
	      struct pt_regs *regs){
  struct abyon_binprm *bprm = kmalloc(sizeof(*bprm),0);
  struct file *file = open_exec(filename);
  bprm->file = file;
  bprm->filename = filename;
  //bprm->mm = alloc_mm();
  bprm->argc = count(argv,bprm->p / sizeof(void*));
  bprm->envc = count(envp,bprm->p / sizeof(void*));

  int retval = prepare_binprm(bprm);
  retval = search_binary_handler(bprm,regs);
  kfree(bprm);
  return retval;
}
