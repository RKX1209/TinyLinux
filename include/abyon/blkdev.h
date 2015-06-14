/*
 * include/abyon/blkdev.h
 * Copyright (C) <2015>  <@RKX1209>
 */
#ifndef _ABYON_BLKDEV_H
#define _ABYON_BLKDEV_H

#include <abyon/bio.h>
#include <abyon/elevator.h>
#include <abyon/fs.h>
struct request_queue;

typedef struct request_queue request_queue_t;
typedef void (request_fn_proc) (request_queue_t*);
typedef int (make_request_fn)(request_queue_t *,struct bio *);

struct request_queue{
  struct list_head queue_head;
  struct request *last_merge;
  elevator_t *elevator;
request_fn_proc *request_fn;
  make_request_fn * make_request_fn;
};

struct request{
  struct list_head queuelist;
  sector_t sector;
  unsigned long nr_sectors;
  struct bio *bio;
  struct bio *biotail;
  
};
static inline request_queue_t *bdev_get_queue(struct block_device *bdev){
  //return bdev->bd_disk->queue;
}

#endif
