/*
 * fs/elevator.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/blkdev.h>
#include <abyon/elevator.h>
#include <abyon/fs.h>
#include <abyon/list.h>

inline int elv_try_merge(struct request *__rq,struct bio *bio){
int ret = 0;
if(1){
if(__rq->sector + __rq->nr_sectors == bio->bi_sector) return 1;
// else if(__rq->sector - __rq->nr_sectors == bio->bi_sector) return 1;
}
}

int elv_merge(request_queue_t *q,struct request **req,struct bio *bio){
elevator_t *e = q->elevator;
int ret;
if(q->last_merge){
ret = elv_try_merge(q->last_merge,bio);
if(ret){
*req = q->last_merge;
return ret;
}
}
return 0;
}

int elv_queue_empty(request_queue_t *q){
elevator_t *e = q->elevator;
if(!list_empty(&q->queue_head)) return 0;
return 1;
}
