/*
 * kernel/fork.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <asm/cache.h>

static kmem_cache_t *task_struct_cachep;
int max_threads;

void fork_init(unsigned long mempages){
  #define ARCH_MIN_TASKALIGN L1_CACHE_BYTES
  task_struct_cachep = kmem_cache_create("task_struct",sizeof(task_struct),
					 ARCH_MIN_TASKALIGN,0,0,0);
  max_threads = mempages / (8 * THREAD_SIZE / PAGE_SIZE);
  if(max_threads < 20) max_threads = 20;
  
}
