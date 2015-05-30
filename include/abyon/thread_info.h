/*
 * include/abyon/thread_info.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ABYON_THREAD_INFO_H
#define _ABYON_THREAD_INFO_H

#include <asm/bitops.h>
#include <asm/thread_info.h>

#define preempt_count() (current_thread_info()->preempt_count)

static inline void set_ti_thread_flag(struct thread_info *ti,int flag){
  __set_bit(flag,&ti->flags);
}
#endif
