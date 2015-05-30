/*
 * fs/exec.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/binfmts.h>
#include <abyon/fs.h>
#include <abyon/ptrace.h>

struct file *open_exec(const char *name){
int err = path_lookup(name,0,0);

}
int do_execve(char *filename,
	      char *argv,
	      char *envp,
	      struct pt_regs *regs){
  struct abyon_binprm *bprm = kmalloc(sizeof(*bprm),0);
struct file *file = open_exec(filename);

}
