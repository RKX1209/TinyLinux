/*
 * fs/binfmt_elf.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/binfmts.h>
#include <abyon/dcache.h>
#include <abyon/fs.h>
#include <abyon/namei.h>
#include <abyon/sched.h>

#include <asm/current.h>
#include <asm/processor.h>
static int load_elf_binary(struct abyon_binprm *bprm, struct pt_regs *regs){
  return 0;
}

static struct abyon_binfmt elf_format = { 
 .load_binary = load_elf_binary
};

static int init_elf_binfmt(void){
  return register_binfmt(&elf_format);
}
