/*
 * kernel/initramfs.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/sched.h>
#include <asm/system.h>
#include <drivers/disp.h>
extern unsigned long _initramfs_start;
extern unsigned long _initramfs_end;

static char * unpack_to_rootfs(char *buf,unsigned len){
  
}

void populate_rootfs(void){
  char *err = unpack_to_rootfs(_initramfs_start,_initramfs_end - _initramfs_start);
}
