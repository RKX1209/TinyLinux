/*
 * fs/read_write.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/binfmts.h>
#include <abyon/dcache.h>
#include <abyon/fs.h>
#include <abyon/namei.h>
#include <abyon/ptrace.h>

#include <asm/processor.h>

int vfs_read(struct file *file, char *buf, unsigned long count, int pos){
  struct inode *inode = file->f_dentry->d_inode;
  int ret = 0;
  if(file->f_op->read)
    ret = file->f_op->read(file,buf,count,pos);
  return ret;
  
}

int vfs_write(struct file *file, char *buf, unsigned long count, int pos){
  struct inode *inode = file->f_dentry->d_inode;
  int ret = 0;
  if(file->f_op->write)
    ret = file->f_op->write(file,buf,count,pos);
  return ret;
  
}

unsigned int sys_read(unsigned int fd,char *buf,int count){
  struct file *file = fget_light(fd,0);
  unsigned int ret = -1;
  if(file){
    unsigned int pos = file->f_pos;
    ret = vfs_read(file,buf,count,pos);
    file->f_pos = pos;
  }
  return ret;
}

unsigned int sys_write(unsigned int fd,char *buf,int count){
  struct file *file = fget_light(fd,0);
  unsigned int ret = -1;
  if(file){
    unsigned int pos = file->f_pos;
    ret = vfs_write(file,buf,count,pos);
    file->f_pos = pos;
  }
  return ret;
}
