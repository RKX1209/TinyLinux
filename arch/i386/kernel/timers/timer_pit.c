/*
 * arch/i386/kernel/timers/timer_pit.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <asm/io.h>
#include <asm/io_ports.h>

#define LATCH 0x2e9c

extern int printk(const char *fmt, ...);

void setup_pit_timer(void){
  unsigned long flags;
  outb_p(0x34,PIT_MODE);		/* binary, mode 2, LSB/MSB, ch 0 */
  //udelay(10);
  outb_p(LATCH & 0xff , PIT_CH0);	/* LSB */
  //udelay(10);
  outb(LATCH >> 8 , PIT_CH0);	/* MSB */
  printk("Initializing PIT... [OK]");
}
