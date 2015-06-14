/*
 * fs/mpage.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/binfmts.h>
#include <abyon/dcache.h>
#include <abyon/elevator.h>
#include <abyon/blkdev.h>
#include <abyon/fs.h>
#include <abyon/mm.h>
#include <abyon/namei.h>
#include <abyon/ptrace.h>

#include <asm/page.h>
#include <asm/processor.h>

typedef int (get_block_t)(struct inode *, sector_t, struct buffer_head *, int);

static struct bio *do_mpage_readpage(struct bio *bio,struct page *page,
				     unsigned nr_pages,sector_t *last_block_in_bio,get_block_t get_block){
  struct inode *inode = page->mapping->host;
  const unsigned blkbits = inode->i_blkbits;
  const unsigned blocks_per_page = PAGE_CACHE_SIZE >> blkbits;
  const unsigned blocksize = 1 << blkbits;
  sector_t blocks[1024];
  sector_t block_in_file = page->index << (PAGE_CACHE_SHIFT - blkbits);
  unsigned page_block = 0;
  struct block_device *bdev = 0;
  for(page_block = 0; page_block < blocks_per_page; page_block++,block_in_file++){
    get_block(inode,block_in_file,0,0);
    blocks[page_block] = 0;
  }
  if(bio == 0){
    
  }
  return bio;
}

static int __make_request(request_queue_t *q,struct bio *bio){
  struct request *req;
  if(elv_queue_empty(q)) ;
  int el_ret = elv_merge(q,&req,bio);
  switch(el_ret){
  case ELEVATOR_BACK_MERGE:
    req->biotail->bi_next = bio;
    req->biotail = bio;
    goto out;
    
  }
 out:
  __generic_unplug_device(q);
  return 0;
}
void __generic_unplug_device(request_queue_t *q){
  q->request_fn(q);
}

void generic_make_request(struct bio *bio){
  int ret;
  request_queue_t *q;
  do{
    q = bdev_get_queue(bio->bi_bdev);
    ret = q->make_request_fn(q,bio);
  }while(ret);
}

static struct bio *mpage_bio_submit(int rw,struct bio *bio){
  //submit_bio(rw,bio);
  generic_make_request(bio);
  return 0;
}

int mpage_readpage(struct page *page, get_block_t get_block){
  struct bio *bio = 0;
  sector_t last_block_in_bio = 0;
  bio = do_mpage_readpage(bio,page,1,&last_block_in_bio,get_block);
  if(bio)
    mpage_bio_submit(0,bio);
  return 0;
}
