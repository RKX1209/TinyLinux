/*
 * include/asm/system.h  
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef __ASM_SYSTEM_H
#define __ASM_SYSTEM_H

/* Operation of special registes */
#define read_cr0()({ \
  unsigned int __ret; \
  __asm__ __volatile__( \
  "movl %%cr0,%0\n\t" \
  :"=r" (__ret)); \
  __ret; \
})

#define write_cr0(x) \
	__asm__ __volatile__("movl %0,%%cr0": :"r" (x));

#define read_cr2()({ \
  unsigned int __ret; \
  __asm__ __volatile__( \
  "movl %%cr2,%0\n\t" \
  :"=r" (__ret)); \
  __ret; \
})

#define write_cr2(x) \
	__asm__ __volatile__("movl %0,%%cr2": :"r" (x));

#define read_cr3()({ \
  unsigned int __ret; \
  __asm__ __volatile__( \
  "movl %%cr3,%0\n\t" \
  :"=r" (__ret)); \
  __ret; \
})

#define write_cr3(x) \
	__asm__ __volatile__("movl %0,%%cr3": :"r" (x));

#define read_cr4()({ \
  unsigned int __ret; \
  __asm__ __volatile__( \
  "movl %%cr4,%0\n\t" \
  :"=r" (__ret)); \
  __ret; \
})

#define write_cr4(x) \
	__asm__ __volatile__("movl %0,%%cr4": :"r" (x));

/* Write memory barrier */
#define wmb() __asm__ __volatile__("sfence");
#define smp_wmb() wmb()


#endif
