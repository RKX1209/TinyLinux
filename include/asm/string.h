/*
 * include/asm/string.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ASM_STRING_H
#define _ASM_STRING_H

static inline void *__memcpy(void *to,void *from ,int n){
  int d0,d1,d2;
  __asm__ __volatile__(
	"rep ; movsl\n\t"
	"movl %4,%%ecx\n\t"
	"andl $3,%%ecx\n\t"
	"rep ; movsb\n\t"
	:"=&c" (d0),"=&D" (d1), "=&S" (d2)
	:"0" (n/4),"g" (n), "1" ((long)to),"2" ((long)from)
	:"memory");
  return (to);
}
static inline void * __memset_generic(void * s, char c,int count){
  int d0,d1;
  __asm__ __volatile__(
	"rep\n\t"
	"stosb"
	: "=&c" (d0), "=&D" (d1)
	:"a" (c),"1" (s),"0" (count)
	:"memory");
  return s;
}

static inline unsigned long strnlen(const char *s, unsigned long count){
  int __res;
  int d0;
  __asm__ __volatile__(
	"movl %2,%0\n\t"
	"jmp 2f\n"
	"1:\tcmpb $0,(%0)\n\t"
	"je 3f\n\t"
	"incl %0\n"
	"2:\tdecl %1\n\t"
	"cmpl $-1,%1\n\t"
	"jne 1b\n"
	"3:\tsubl %2,%0"
	:"=a" (__res), "=&d" (d0)
	:"c" (s), "1" (count)
	:"memory");
  return __res;
}

#define __memset(s,c,count) \
  (__memset_generic((s),(c),(count)))


#endif
