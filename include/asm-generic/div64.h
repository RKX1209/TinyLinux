/*
  include/asm-generic/div64.h
  * Copyright (C) <2014>  <@RKX1209>
  */

#ifndef _ASM_GENERIC_DIV64_H
#define _ASM_GENERIC_DIV64_H


unsigned long long __udivmoddi4(unsigned long long num, unsigned long long den, unsigned long long *rem_p){
  printk("%d,%d\n",num,den);
  unsigned long long quot = 0, qbit = 1;
  while ( (unsigned long long)den >= 0 ) {
    den <<= 1;
    qbit <<= 1;
  }
 
  while ( qbit ) {
    if ( den <= num ) {
      num -= den;
      quot += qbit;
    }
    den >>= 1;
    qbit >>= 1;
  }
 
  if ( rem_p )
    *rem_p = num;
 
  return quot;
}


unsigned long long __udivdi3(unsigned long long num, unsigned long long den){
printk("%d,%d\n",num,den);
  return __udivmoddi4(num, den, 0);
}
unsigned long long __umoddi3(unsigned long long num, unsigned long long den){
  unsigned long long v;
  (void) __udivmoddi4(num, den, &v);
  return v;
}

unsigned long long divmodk(unsigned long long num, unsigned long long den, unsigned long long *rem_p){
  //printk("%d,%d\n",num,den);
  unsigned long long quot = 0, qbit = 1;
  while ( (unsigned long long)den >= 0 ) {
    den <<= 1;
    qbit <<= 1;
  }
 
  while ( qbit ) {
    if ( den <= num ) {
      num -= den;
      quot += qbit;
    }
    den >>= 1;
    qbit >>= 1;
  }
 
  if ( rem_p )
    *rem_p = num;
 
  return quot;
}


unsigned long long divk(unsigned long long num, unsigned long long den){
printk("%d,%d\n",num,den);
  return divmodk(num, den, 0);
}

unsigned long long modk(unsigned long long num, unsigned long long den){
printk("%d,%d\n",num,den);
  unsigned long long v;
  (void) divmodk(num, den, &v);
  return v;
}
#endif
