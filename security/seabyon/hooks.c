/*
 * security/seabyon/hooks.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/security.h>
#include "av_permission.h"
#include "objsec.h"

struct security_operations *secondary_ops = 0;
struct security_operations *original_ops = 0;
extern struct security_operations *security_ops;

static int task_has_perm(struct task_struct *tsk1,
			 struct task_struct *tsk2,
			 unsigned int perms){
  struct task_security_struct *tsec1,*tsec2;
  tsec1 = tsk1->security;
  tsec2 = tsk2->security;
  return avc_has_perm(tsec1->sid,tsec2->sid,
		      0,perms,0);
}
static int seabyon_ptrace(struct task_struct *parent,struct task_struct *child){
  struct task_security_struct *psec = parent->security;
  struct task_security_struct *csec = child->security;

  int rc = secondary_ops->ptrace(parent,child);
  if(rc) return rc;
  rc = task_has_perm(parent,child,PROCESS__PTRACE);
  return rc;
}

static struct security_operations seabyon_ops = {
  .ptrace = seabyon_ptrace
};

static int seabyon_init(void){
  original_ops = secondary_ops = security_ops;
avc_init();
  register_security(&seabyon_ops);
  return 0;
}
