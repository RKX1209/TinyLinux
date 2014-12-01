/*
 * arch/i386/kernel/setup.c
 * Copyright (C) <2014>  <@RKX1209>
 */

void paging_init(void);

/* setup arch part */
void setup_arch(){
  
  /* setup page tables */  
  paging_init();
}
