/*
 * security/seabyon/objsec.h
 * Copyright (C) <2015>  <@RKX1209>
 */

#ifndef _SEABYON_OBJSEC_H
#define _SEABYON_OBJSEC_H
#include <abyon/sched.h>

struct task_security_struct{
  unsigned long magic;
  struct task_struct *task;
  unsigned int osid;
  unsigned int sid;
  unsigned int exec_sid;
  unsigned int create_sid;
  unsigned int ptrace_sid;
};
#endif
