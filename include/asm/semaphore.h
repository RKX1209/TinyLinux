/*
 * include/asm/semaphore.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef _ASM_SEMAPHORE_H
#define _ASM_SEMAPHORE_H

#include <asm/atomic.h>

#include <abyon/wait.h>

struct semaphore{
  atomic_t count;
  int sleepers;
  wait_queue_head_t wait;
};

static inline void sema_init(struct semaphore *sem,int val){
  atomic_set(&sem->count,val);
  sem->sleepers = 0;
  init_waitqueue_head(&sem->wait);
}

static inline void init_MUTEX(struct semaphore *sem){
  sema_init(sem,1);
}
#endif
