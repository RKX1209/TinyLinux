/*
 * fs/ext2/inode.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/binfmts.h>
#include <abyon/dcache.h>
#include <abyon/fs.h>
#include <abyon/namei.h>
#include <abyon/ptrace.h>

#include <asm/processor.h>

static int ext2_readpage(struct file *file,struct page *page){
  return mpage_readpage(page,ext2_get_block);
}

struct address_space_operations ext2_aops = {
  .readpage = ext2_readpage
};
