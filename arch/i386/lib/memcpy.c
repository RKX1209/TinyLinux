/*
 * arch/i386/lib/memcpy.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <asm/string.h>

void *memcpy(void *to, void *from, int n)
{
  return __memcpy(to, from, n);
}

void *memset(void *s, int c, int count)
{
  return __memset(s, c, count);
}
