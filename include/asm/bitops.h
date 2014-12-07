/*
 * include/asm/bitops.h
 * Copyright (C) <2014>  <@RKX1209>
 */

static inline int test_and_set_bit(int nr, volatile unsigned long * addr)
{
  int oldbit;

  __asm__(
	  "btsl %2,%1\n\tsbbl %0,%0"
	  :"=r" (oldbit),"=m" (addr)
	  :"Ir" (nr));
  return oldbit;
}


static inline int test_and_clear_bit(int nr, volatile unsigned long * addr){
  int oldbit;
  __asm__ __volatile__ (
	"lock ; \n\t" 
	"btrl %2,%1\n\t" 
	"sbbl %0,%0"
	: "=r" (oldbit), "=m" (addr)
	: "Ir" (nr) : "memory");
  return oldbit;
}
