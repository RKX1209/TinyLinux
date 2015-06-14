/*
 * include/abyon/bio.h
 * Copyright (C) <2015>  <@RKX1209>
 */
#ifndef _ABYON_BIO_H
#define _ABYON_BIO_H

#include <abyon/fs.h>

struct bio{
  sector_t bi_sector;
  struct block_device *bi_bdev;
  struct bio *bi_next;
};

#endif
