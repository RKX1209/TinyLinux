/*
 * arch/i386/mm/discontig.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <asm/bootmem.h>


extern void setup_bootmem_allocator(void);
/* Setup memory informations */
unsigned long setup_memory(void){
  setup_bootmem_allocator();
}
