/*
 * include/abyon/list.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ABYON_LIST_H
#define _ABYON_LIST_H

struct list_head{
struct list_head *next,*prev;
};

#define INIT_LIST_HEAD(ptr) do {					\
(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

static inline void __list_add(struct list_head *add,
			      struct list_head *prev,
			      struct list_head *next){
  next->prev = add;
  add->next = next;
  add->prev = prev;
  prev->next = add;
}

static inline void list_add(struct list_head *add, struct list_head *head)
{
  __list_add(add, head, head->next);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
  next->prev = prev;
  prev->next = next;
}

#endif
