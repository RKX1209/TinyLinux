/*
 * mm/mmap.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/mm.h>
#include <abyon/rbtree.h>
#include <abyon/sched.h>

void __vma_link_rb(struct mm_struct *mm,struct vm_area_struct *vma,
		   struct rb_node **rb_link, struct rb_node *rb_parent){
  rb_link_node(&vma->vm_rb,rb_parent,rb_link);
  rb_insert_color(&vma->vm_rb,&mm->mm_rb);
}
