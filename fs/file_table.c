/*
 * fs/file_table.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/file.h>
#include <abyon/fs.h>

#include <asm/current.h>

struct files_stat_struct files_stat = {
  .max_files = NR_FILE
};

struct file *fget_light(unsigned int fd,int *fput_needed){
  /* struct files_struct *files = current->files; */
  /* return files[fd]; */
}
