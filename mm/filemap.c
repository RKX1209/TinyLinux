/*
 * mm/filemap.c
 * Copyright (C) <2015>  <@RKX1209>
 */
#include <abyon/file.h>
#include <abyon/fs.h>
#include <abyon/uio.h>

#include <asm/page.h>

struct page *find_get_page(struct address_space *mapping,unsigned long offset){
  
}

void do_generic_file_read(struct file *filp,unsigned int *ppos,
			  read_descriptor_t *desc,int dumm){
  struct address_space *mapping = filp->f_dentry->d_inode->i_mapping;
  struct inode *inode = mapping->host;
  unsigned long index = *ppos >> PAGE_CACHE_SHIFT;
  unsigned long offset = *ppos & ~PAGE_CACHE_SHIFT;
  for(;;){
    struct page *page = find_get_page(mapping,index);
    unsigned long end_index = inode->i_size >> PAGE_CACHE_SHIFT;
    if(index > end_index) break;
    unsigned long nr,ret;
    nr = PAGE_CACHE_SIZE;
    nr = nr - offset;
    mapping->a_ops->readpage(filp,page);
  }
}

int file_read_actor(read_descriptor_t *desc, struct page *page,
		    unsigned long offset, unsigned long size){
  
}
unsigned int __generic_file_aio_read(struct kiocb *iocb,const struct iovec *iov,
				     unsigned long nr_segs,unsigned int *ppos){
  unsigned long seg;
  unsigned int count;
  for(seg = 0; seg < nr_segs; seg++){
    const struct iovec *iv = &iov[seg];
    count += iv->iov_len;
  }
  if(count){
    for(seg = 0; seg < nr_segs; seg++){
      read_descriptor_t desc;
      desc.written = 0;
      desc.buf = iov[seg].iov_base;
      desc.count = iov[seg].iov_len;
      if(desc.count == 0)
	continue;
      do_generic_file_read(filp,ppos,&desc,0);
    }
  }
  return retval;
}

unsigned int generic_file_read(struct file *filp,char *buf,
			       unsigned int count,int *ppos){
  struct iovec local_iov = { .iov_base = buf, .iov_len = count };
  struct kiocb kiocb;
  init_sync_kiocb(&kiocb,filp);
  int ret = __generic_file_aio_read(&kiocb,&local_iov,1,ppos);
  return ret;
}
