/*
 * include/abyon/namespace.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ABYON_NAMESPACE_H
#define _ABYON_NAMESPACE_H

struct namespace{
  unsigned int count;
  struct vfsmount *root;
  struct list_head list;
};

static inline void get_namespace(struct namespace *namespace){
  namespace->count++;
}
#endif
