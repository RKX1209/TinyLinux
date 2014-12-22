/*
 * include/abyon/timer.h
 * Copyright (C) <2014>  <@RKX1209>
 */


#ifndef _ABYON_TIMER_H
#define _ABYON_TIMER_H

struct timer_base_s;
struct timer_list{
struct list_head entry;
unsigned long expires;
void (*function)(unsigned long);
unsigned long data;
struct timer_base_s *base;
};

#endif
