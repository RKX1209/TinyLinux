/*
 * include/asm/e820.h
 * Copyright (C) <2014>  <@RKX1209>
 */


#ifndef __E820_HEADER
#define __E820_HEADER

#define E820MAX	128

#define E820_RAM	1
#define E820_RESERVED	2
#define E820_ACPI	3
#define E820_NVS	4

struct e820map {
    int nr_map;
    struct e820entry {
	unsigned long long addr;	/* start of memory segment */
	unsigned long long size;	/* size of memory segment */
	unsigned long type;		/* type of memory segment */
    } map[E820MAX];
};


struct e820map e820;

#endif
