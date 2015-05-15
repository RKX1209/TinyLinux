/*
 * fs/filesystem.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <asm/string.h>
#include <abyon/fs.h>
#include <abyon/list.h>

extern struct file_system_type *file_systems;
static struct file_system_type **find_filesystem(const char *name){
  struct file_system_type **p;
  for(p = &file_systems; *p; p = &(*p)->next)
    if(strcmp((*p)->name,name) == 0) break;
  return p;
}

int register_filesystem(struct file_system_type *fs){
  INIT_LIST_HEAD(&fs->fs_supers);
  struct file_system_type **p = find_filesystem(fs->name);
  if(!*p) *p = fs;
  return 0;
}
