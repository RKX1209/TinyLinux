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

void init_e820(){
  e820.nr_map = 6;
  e820.map[0] = (struct e820entry){ 0x00000000,0x0009ffff, E820_RAM };
  e820.map[1] = (struct e820entry){ 0x000f0000,65536, E820_RESERVED };
  e820.map[2] = (struct e820entry){ 0x00100000,133103616, E820_RAM };
  e820.map[3] = (struct e820entry){ 0x07ff0000,125841408, E820_ACPI };
  e820.map[4] = (struct e820entry){ 0x07ff3000,53248, E820_ACPI };
  e820.map[5] = (struct e820entry){ 0xffff0000,65536, E820_RESERVED };  
}

#endif
