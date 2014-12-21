/*
 * include/asm/i8259.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef __ASM_I8259_H__
#define __ASM_I8259_H__

unsigned int cached_irq_mask = 0xf9ef;

#define __byte(x,y) 		(((unsigned char *) &(y))[x])
/* #define cached_master_mask	(__byte(0, cached_irq_mask)) */
/* #define cached_slave_mask	(__byte(1, cached_irq_mask)) */
#define cached_master_mask	0xf9
#define cached_slave_mask	0xef


extern void init_8259A(int auto_eoi);
extern void enable_8259A_irq(unsigned int irq);
extern void disable_8259A_irq(unsigned int irq);
extern unsigned int startup_8259A_irq(unsigned int irq);

#endif
