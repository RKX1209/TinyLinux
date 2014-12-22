/*
 * include/abyon/kernel.h
 * Copyright (C) <2014>  <@RKX1209>
 */


#ifndef _LINUX_KERNEL_H
#define _LINUX_KERNEL_H


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
#endif
