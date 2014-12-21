/*
 * include/asm/io.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef __ASM_IO_H__
#define __ASM_IO_H__

static inline void outb(unsigned short port, unsigned char val){
  asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

#define outb_p outb
#endif
