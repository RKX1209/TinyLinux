/*
 * include/abyon/kernel.h
 * Copyright (C) <2014>  <@RKX1209>
 */


#ifndef _ABYON_KERNEL_H
#define _ABYON_KERNEL_H


#define ALIGN(x,a) ( ( (x)+(a)-1)&~( (a)-1) )
#define fastcall        __attribute__((regparm(3)))

#define offsetof(TYPE, MEMBER) ((unsigned long) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define typecheck(type,x) \
({	type __dummy; \
	typeof(x) __dummy2; \
	(void)(&__dummy == &__dummy2); \
	1; \
})

#define do_div(n,base) ({				\
      unsigned int __base = (base);			\
      unsigned int  rem;				\
      rem = modk(((unsigned long long )(n)), __base);	\
      (n) = divk(((unsigned long long )(n)), __base);	\
      rem;						\
    })

static inline int long_log2(unsigned long x) {

  int r = 0;
  for (x >>= 1; x > 0; x >>= 1)
    r++;
  return r;
}
unsigned long long divmodk(unsigned long long num, unsigned long long den, unsigned long long *rem_p);
unsigned long long divk(unsigned long long num, unsigned long long den);
unsigned long long modk(unsigned long long num, unsigned long long den);
#endif
