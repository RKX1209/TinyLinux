/*
 * fs/file_table.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/fs.h>

struct files_stat_struct files_stat = {
  .max_files = NR_FILE
};
