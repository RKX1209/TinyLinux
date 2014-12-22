/*
 * include/abyon/thread_info.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ABYON_THREAD_INFO_H
#define _ABYON_THREAD_INFO_H

#include <asm/thread_info.h>
#define preempt_count() (current_thread_info()->preempt_count)
#endif
