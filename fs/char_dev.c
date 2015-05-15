/*
 * fs/char_dev.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/fs.h>
#include <abyon/fs_struct.h>
#include <abyon/init_task.h>
#include <abyon/kobj_map.h>
#include <abyon/list.h>
#include <abyon/mount.h>
#include <abyon/namespace.h>
#include <abyon/sched.h>

#include <asm/current.h>
#include <asm/string.h>

static struct kobj_map *cdev_map;

static struct kobject *base_probe(unsigned long dev, int *part,void *data){
return 0;
}
void chrdev_init(void){
cdev_map = kobj_map_init(base_probe);

}
