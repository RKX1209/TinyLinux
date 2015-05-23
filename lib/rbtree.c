/*
 * lib/rbtree.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/rbtree.h>

static void __rb_rotate_left(struct rb_node *node,struct rb_root *root){
  struct rb_node *right = node->rb_right;
  if((node->rb_right = right->rb_left))
    right->rb_left->rb_parent = node;
  right->rb_left = node;
  if((right->rb_parent = node->rb_parent)){
    if(node == node->rb_parent->rb_left)
      node->rb_parent->rb_left = right;
    else
      node->rb_parent->rb_right = right;
  }else
    root->rb_node = right;
  node->rb_parent = right;
}

static void __rb_rotate_right(struct rb_node *node, struct rb_root *root) {
  struct rb_node *left = node->rb_left;
  if ((node->rb_left = left->rb_right))
    left->rb_right->rb_parent = node;
  left->rb_right = node;

  if((left->rb_parent = node->rb_parent)){
      if (node == node->rb_parent->rb_right)
	node->rb_parent->rb_right = left;
      else
	node->rb_parent->rb_left = left;
  }else
    root->rb_node = left;
  node->rb_parent = left;

}
void rb_insert_color(struct rb_node *node, struct rb_root *root){
  struct rb_node *parent,*gparent;
  while((parent = node->rb_parent) && parent->rb_color == RB_RED){
    gparent = parent->rb_parent;
    if(parent == gparent->rb_left){
      {
	struct rb_node *uncle = gparent->rb_right;
	if(uncle && uncle->rb_color == RB_RED){
	  uncle->rb_color = RB_BLACK;
	  parent->rb_color = RB_BLACK;
	  gparent->rb_color = RB_RED;
	  node = gparent;
	  continue;
	}
      }
      if(parent->rb_right == node){
	__rb_rotate_left(parent,root);
	struct rb_node *tmp;
	tmp = parent;
	parent = node;
	node = tmp;
      }
      parent->rb_color = RB_BLACK;
      gparent->rb_color = RB_RED;
      __rb_rotate_right(gparent,root);
    } else{
      {
	struct rb_node *uncle = gparent->rb_left;
	if(uncle && uncle->rb_color == RB_RED){
	  uncle->rb_color = RB_BLACK;
	  parent->rb_color = RB_BLACK;
	  gparent->rb_color = RB_RED;
	  node = gparent;
	  continue;
	}
      }
      if(parent->rb_left == node){
	__rb_rotate_right(parent,root);
	struct rb_node *tmp;
	tmp = parent;
	parent = node;
	node = tmp;
      }
      parent->rb_color = RB_BLACK;
      gparent->rb_color = RB_RED;
      __rb_rotate_right(gparent,root);
      
    }
  }
  root->rb_node->rb_color = RB_BLACK;
}
