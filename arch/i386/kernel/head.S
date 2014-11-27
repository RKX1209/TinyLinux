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

/*
 32-bit kernel entrypoint
*/
	
.text	

.global startup_32
.global kernel_main

#include <asm/segment.h>

//.org 0x280000
startup_32:
	/* Set temporary segments */
	cld
	lgdt boot_gdt_descr
	movl $(__BOOT_DS),%eax
	movl %eax,%ds
	movl %eax,%es
	movl %eax,%fs
	movl %eax,%gs
	movl %eax,%ss

	/* Clear BSS */
	xorl %eax, %eax
	movl $_bss_start,%edi
	movl $_bss_end,%ecx
	subl %edi,%ecx
	shrl $2,%ecx
	rep ; stosl
	
	/* Enable virtual paging */
	/*movl $swapper_pg_dir,%eax
	movl %eax,%cr3 
	movl %cr0,%eax
	orl $0x80000000,%eax
	movl %eax,%cr0 */
	ljmp $__BOOT_CS,$1f

1:

	//lss stack_start,%esp//[*] Bug
	/* init eflags */
	pushl $0
	popfl
	
	/* Set up idt */
	call setup_idt

	/* Jump to kernel */
	call kernel_main

/* sets up a idt with 256 entries pointing to
 * ignore_int(default interupt handler)
 */
setup_idt:
	lea ignore_int,%edx
	movl $(__KERNEL_CS << 16),%eax
	movw %dx,%ax
	movw $0x8e00,%dx

	lea idt_table,%edi
	mov $256,%ecx
per_idt:
	movl %eax,(%edi)
	movl %edx,4(%edi)
	addl $8,%edi
	dec %ecx
	jne per_idt
	ret
	
/* default interrupt handler */	
ignore_int:
	cld
	iret
	
	
.section .bss
swapper_pg_dir:
	.fill 1024,4,0
empty_zero_page:
	.fill 4096,1,0
idt_table:	.fill 256,8,0		/* idt is uninitialized */
	
.data
stack_start:
	.long __BOOT_DS

boot_gdt_descr:
	.word __BOOT_DS + 7
	.long boot_gdt_table
	.word 0

idt_descr:
	.word IDT_ENTRIES*8-1
	.long idt_table
	.word 0
	
cpu_gdt_descr:
	.word GDT_ENTRIES*8-1
	.long cpu_gdt_table	

boot_gdt_table:	
	.fill GDT_ENTRY_BOOT_CS,8,0
	.quad 0x00cf9a000000ffff	/* kernel 4GB code at 0x00000000 */
	.quad 0x00cf92000000ffff	/* kernel 4GB data at 0x00000000 */
	
cpu_gdt_table:	
	.quad 0x0000000000000000	/* NULL descriptor */
	.quad 0x0000000000000000	/* 0x0b reserved */
	.quad 0x0000000000000000	/* 0x13 reserved */
	.quad 0x0000000000000000	/* 0x1b reserved */
	.quad 0x0000000000000000	/* 0x20 unused */
	.quad 0x0000000000000000	/* 0x28 unused */
	.quad 0x0000000000000000	/* 0x33 TLS entry 1 */
	.quad 0x0000000000000000	/* 0x3b TLS entry 2 */
	.quad 0x0000000000000000	/* 0x43 TLS entry 3 */
	.quad 0x0000000000000000	/* 0x4b reserved */
	.quad 0x0000000000000000	/* 0x53 reserved */
	.quad 0x0000000000000000	/* 0x5b reserved */

	.quad 0x00cf9a000000ffff	/* 0x60 kernel 4GB code at 0x00000000 */
	.quad 0x00cf92000000ffff	/* 0x68 kernel 4GB data at 0x00000000 */
	.quad 0x00cffa000000ffff	/* 0x73 user 4GB code at 0x00000000 */
	.quad 0x00cff2000000ffff	/* 0x7b user 4GB data at 0x00000000 */

	.quad 0x0000000000000000	/* 0x80 TSS descriptor */
	.quad 0x0000000000000000	/* 0x88 LDT descriptor */

	/* Segments used for calling PnP BIOS */
	.quad 0x00c09a0000000000	/* 0x90 32-bit code */
	.quad 0x00809a0000000000	/* 0x98 16-bit code */
	.quad 0x0080920000000000	/* 0xa0 16-bit data */
	.quad 0x0080920000000000	/* 0xa8 16-bit data */
	.quad 0x0080920000000000	/* 0xb0 16-bit data */
	/*
	 * The APM segments have byte granularity and their bases
	 * and limits are set at run time.
	 */
	.quad 0x00409a0000000000	/* 0xb8 APM CS    code */
	.quad 0x00009a0000000000	/* 0xc0 APM CS 16 code (16 bit) */
	.quad 0x0040920000000000	/* 0xc8 APM DS    data */

	.quad 0x0000920000000000	/* 0xd0 - ESPFIX 16-bit SS */
	.quad 0x0000000000000000	/* 0xd8 - unused */
	.quad 0x0000000000000000	/* 0xe0 - unused */
	.quad 0x0000000000000000	/* 0xe8 - unused */
	.quad 0x0000000000000000	/* 0xf0 - unused */
	.quad 0x0000000000000000	/* 0xf8 - GDT entry 31: double-fault TSS */
	
