/*
 * include/abyon/wait.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef _ABYON_WAIT_H
#define _ABYON_WAIT_H

#include <abyon/list.h>

typedef struct __wait_queue wait_queue_t;
typedef int (*wait_queue_func_t)(wait_queue_t *wait, unsigned mode, int sync, void *key);
int default_wake_function(wait_queue_t *wait, unsigned mode, int sync, void *key);

struct task_struct;
struct __wait_queue{
  unsigned int flags;
  struct task_struct *task;
  wait_queue_func_t func;
  struct list_head task_list;
};

struct __wait_queue_head{
  struct list_head task_list;
};

typedef struct __wait_queue_head wait_queue_head_t;

static inline void init_waitqueue_head(wait_queue_head_t *q){
  INIT_LIST_HEAD(&q->task_list);
}

#endif
