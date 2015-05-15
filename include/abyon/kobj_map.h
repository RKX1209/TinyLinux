/*
 * fs/char_dev.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef __ABYON_KOBJ_MAP_H
#define __ABYON_KOBJ_MAP_H

typedef struct kobject *kobj_probe_t(unsigned long, int *, void *);
struct kobj_map *kobj_map_init(kobj_probe_t *);

#endif
