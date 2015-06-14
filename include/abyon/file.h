/*
 * include/abyon/file.h
 * Copyright (C) <2015>  <@RKX1209>
 */
#ifndef _ABYON_FILE_H
#define _ABYON_FILE_H

#define NR_OPEN_DEFAULT 128

struct files_struct{
  struct file *fd_array[NR_OPEN_DEFAULT];
};

#endif
