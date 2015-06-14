/*
 * include/abyon/binfmts.h
 * Copyright (C) <2015>  <@RKX1209>
 */
#ifndef _ABYON_BINFMTS_H
#define _ABYON_BINFMTS_H

#include <abyon/mm.h>

#define MAX_ARG_PAGES 32
#define BINPRM_BUF_SIZE 128

struct abyon_binfmt{
  struct abyon_binfmt *next;
  int (*load_binary)(struct abyon_binprm *,struct pt_regs *regs);
  int (*core_dump)(long signr,struct pt_regs *regs,struct file *file);
};

struct abyon_binprm{
  char buf[BINPRM_BUF_SIZE];
  struct page *page[MAX_ARG_PAGES];
  struct mm_struct *mm;
  unsigned long p;
  struct file *file;
  int e_uid,e_gid;
  char *filename;
  char *interp;
  int argc,envc;
};

#endif
