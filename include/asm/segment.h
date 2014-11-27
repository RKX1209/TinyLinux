   /*Copyright (C) <2014>  <@RKX1209> 

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>
   */


#ifndef _ASM_SEGMENT_H
#define _ASM_SEGMENT_H

/* GDT entry (Kernel space) */
#define GDT_ENTRY_KERNEL_BASE 12


#define GDT_ENTRY_KERNEL_CS (GDT_ENTRY_KERNEL_BASE + 0)//Kernel CS
#define __KERNEL_CS (GDT_ENTRY_KERNEL_CS * 8)

#define GDT_ENTRY_KERNEL_DS (GDT_ENTRY_KERNEL_BASE + 1)//Kernel DS
#define __KERNEL_DS (GDT_ENTRY_KERNEL_DS * 8)

/* GDT Size */
#define GDT_ENTRIES 32
#define GDT_SIZE (GDT_ENTRIES * 8)

/* Simple GDT entries used in booting process */
#define GDT_ENTRY_BOOT_CS		2
#define __BOOT_CS	(GDT_ENTRY_BOOT_CS * 8)

#define GDT_ENTRY_BOOT_DS		(GDT_ENTRY_BOOT_CS + 1)
#define __BOOT_DS	(GDT_ENTRY_BOOT_DS * 8)

/* IDT Size */
#define IDT_ENTRIES 256

#endif
