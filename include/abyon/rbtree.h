/*
 * include/abyon/rbtree.h
 * Copyright (C) <2015>  <@RKX1209>
 */

#ifndef _ABYON_RBTREE_H
#define _ABYON_RBTREE_H

#define RB_RED 0
#define RB_BLACK 1

struct rb_node{
  struct rb_node *rb_parent;
  int rb_color;
  struct rb_node *rb_right;
  struct rb_node *rb_left;
};

struct rb_root{
  struct rb_node *rb_node;
};

static inline void rb_link_node(struct rb_node *node,struct rb_node *parent,
				struct rb_node ** rb_link){
  node->rb_parent = parent;
  node->rb_color = RB_RED;
  node->rb_left = node->rb_right = 0;
  *rb_link = node;
}

#endif
