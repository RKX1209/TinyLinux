/*
 * include/asm/bitops.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef _ASM_BITOPS_H
#define _ASM_BITOPS_H

static inline int test_and_set_bit(int nr, volatile unsigned long * addr)
{
  unsigned long index = nr / 32;
  unsigned long * tmp = (unsigned long *)&((addr)[index]);
  unsigned char bit = 32 - (nr % 32) - 1;
  *tmp = *tmp | (1 << bit);
  //printk("%d=>index:%d bit:%d",nr,index,bit);
    
  /* unsigned long oldbit; */
  /* __asm__( */
  /* 	  "btsl %2,%1\n\tsbbl %0,%0" */
  /* 	  :"=r" (oldbit),"=m" (addr) */
  /* 	  :"Ir" (nr)); */
  return 1;
}


static int test_and_clear_bit(int nr, volatile unsigned long * addr){
  unsigned long index = nr / 32;
  unsigned long * tmp = (unsigned long *)&((addr)[index]);
  unsigned char bit = 32 - (nr % 32) - 1;
  *tmp = *tmp & ~(1 << bit);
 
  /* __asm__ __volatile__( */
  /* 	"btrl %2,%1\n\tsbbl %0,%0" */
  /* 	:"=r" (oldbit),"=m" (addr) */
  /* 	:"Ir" (nr) : "memory"); */
  return 1;  
}

static inline int find_first_zero_bit(const unsigned long *addr, unsigned size)
{
  int d0, d1, d2;
  int res;

  if (!size)
    return 0;
  
  __asm__ __volatile__(
    "movl $-1,%%eax\n\t"
    "xorl %%edx,%%edx\n\t"
    "repe; scasl\n\t"
    "je 1f\n\t"
    "xorl -4(%%edi),%%eax\n\t"
    "subl $4,%%edi\n\t"
    "bsfl %%eax,%%edx\n"
    "1:\tsubl %%ebx,%%edi\n\t"
    "shll $3,%%edi\n\t"
    "addl %%edi,%%edx"
    :"=d" (res), "=&c" (d0), "=&D" (d1), "=&a" (d2)
    :"1" ((size + 31) >> 5), "2" (addr), "b" (addr) : "memory");
  return res;
}

static int constant_test_bit(int nr, const volatile unsigned long *addr){
  unsigned long index = nr / 32;
  unsigned long * tmp = (unsigned long *)&((addr)[index]);  
  unsigned char bit = 32 - (nr % 32) - 1;
  
  if(*tmp & (1 << bit)) return 1;
  return 0;
}

static inline void __set_bit(int nr, volatile unsigned long * addr) {

  __asm__(
    "btsl %1,%0"
    :"=m" (addr)
    :"Ir" (nr));

}
static inline void __clear_bit(int nr, volatile unsigned long * addr){
  __asm__ __volatile__(
    "btrl %1,%0"
    :"=m" (addr)
    :"Ir" (nr));
}
#define test_bit(nr,addr)			\
  constant_test_bit((nr),(addr))

static inline int ffs(int x){
  int r;

  __asm__("bsfl %1,%0\n\t"
	  "jnz 1f\n\t"
	  "movl $-1,%0\n"
	  "1:" : "=r" (r) : "rm" (x));
  return r+1;
}
static inline unsigned long __ffs(unsigned long word){
  __asm__("bsfl %1,%0"
	  :"=r" (word)
	  :"rm" (word));
  return word;
}

static inline int sched_find_first_bit(const unsigned long *b){
  if (b[0])
    return __ffs(b[0]);
  if (b[1])
    return __ffs(b[1]) + 32;
  if (b[2])
    return __ffs(b[2]) + 64;
  if (b[3])
    return __ffs(b[3]) + 96;
  return __ffs(b[4]) + 128;
}

#endif
