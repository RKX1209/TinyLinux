/*
 * include/asm/highmem.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ASM_HIGHMEM_H
#define _ASM_HIGHMEM_H

/* head (virtual) address of HIGHMEM */
extern pte_t *pkmap_page_table;


#define LAST_PKMAP 1024

#define PKMAP_BASE 0xff800000UL

#endif
