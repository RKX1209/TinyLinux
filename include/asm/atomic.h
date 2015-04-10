/*
 * include/asm/atomic.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef _ASM_ATOMIC_H
#define _ASM_ATOMIC_H

#define LOCK ""

typedef struct { volatile int counter; } atomic_t;

#define ATMOIC_INIT(i) { (i) }
#define atomic_read(v) ((v)->counter)
#define atomic_set(v,i) (((v)->counter) = (i))

static inline void atomic_add(int i,atomic_t *v){
  __asm__ __volatile__(
		       LOCK "addl %1,%0"
		       :"=m" (v->counter)
		       :"ir" (i), "m" (v->counter));
}

static inline void atomic_sub(int i,atomic_t *v){
  __asm__ __volatile__(
		       LOCK "subl %1,%0"
		       :"=m" (v->counter)
		       :"ir" (i), "m" (v->counter));
}

static __inline__ void atomic_inc(atomic_t *v)
{
	__asm__ __volatile__(
		LOCK "incl %0"
		:"=m" (v->counter)
		:"m" (v->counter));
}

static __inline__ void atomic_dec(atomic_t *v)
{
	__asm__ __volatile__(
		LOCK "decl %0"
		:"=m" (v->counter)
		:"m" (v->counter));
}

#endif
