/*
 * include/asm/tlbflush.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _I386_TLBFLUSH_H
#define _I386_TLBFLUSH_H

#include <asm/processor.h>

#define __flush_tlb()						\
  do {								\
    unsigned int tmpreg;					\
								\
    __asm__ __volatile__(					\
			 "movl %%cr3, %0;              \n"	\
			 "movl %0, %%cr3;  # flush TLB \n"	\
			 : "=r" (tmpreg)			\
			 :: "memory");				\
  } while (0)

# define __flush_tlb_all()			\
  do {						\
    __flush_tlb();				\
  } while (0)
#endif
