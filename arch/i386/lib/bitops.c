/*
 * arch/i386/lib/bitops.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <asm/bitops.h>

int find_next_zero_bit(const unsigned long *addr, int size, int offset){
  unsigned long *p = ((unsigned long *) addr) + (offset >> 5);
  int set = 0, bit = offset & 31, res;

  if (bit) {
    __asm__("bsfl %1,%0\n\t"
	    "jne 1f\n\t"
	    "movl $32, %0\n"
	    "1:"
	    : "=r" (set)
	    : "r" (~(*p >> bit)));
    if (set < (32 - bit))
      return set + offset;
    set = 32 - bit;
    p++;
  }
  res = find_first_zero_bit (p, size - 32 * (p - (unsigned long *) addr));
  return (offset + set + res);
  
}
