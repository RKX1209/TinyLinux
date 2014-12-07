/*
 * arch/i386/kernel/head.S
 * Copyright (C) <2014>  <@RKX1209>
 */

	
/*
 32-bit kernel entrypoint
*/


.text	

.global startup_32
.global kernel_main
.global	init_pg_tables_end

#include <asm/segment.h>
#define __PAGE_OFFSET 0xC0000000
#define INIT_MAP_BEYOND_END	(128*1024)
	
/* "foo - __PAGE_OFFSET" expression must be used
 * if you wanna change virtual address to physical address
 * in "straight map area"
 */	
startup_32:
	/* Set temporary segments */
	cld
	lgdt boot_gdt_descr - __PAGE_OFFSET
	movl $(__BOOT_DS),%eax
	movl %eax,%ds
	movl %eax,%es
	movl %eax,%fs
	movl %eax,%gs

	/* Clear BSS */
	xorl %eax, %eax
	movl $_bss_start - __PAGE_OFFSET,%edi
	movl $_bss_end - __PAGE_OFFSET,%ecx
	subl %edi,%ecx
	shrl $2,%ecx
	rep ; stosl

	/* Set global directory for (8-MB) temporary memory
	 * EAX = page frame number
	 */
page_pde_offset = (__PAGE_OFFSET >> 22);

	movl $(pg0 - __PAGE_OFFSET), %edi
	movl $(swapper_pg_dir - __PAGE_OFFSET), %edx
	movl $0x007, %eax			/* 0x007 = PRESENT+RW+USER */
10:
	leal 0x007(%edi),%ecx			/* Create PDE entry */
	movl %ecx,(%edx)			/* Store identity PDE entry */
	movl %ecx,page_pde_offset(%edx)		/* Store kernel PDE entry */
	addl $4,%edx
	movl $1024, %ecx
11:
	stosl
	addl $0x1000,%eax
	loop 11b
	//movl %edi,(init_pg_tables_end - __PAGE_OFFSET)

10:
	leal 0x007(%edi),%ecx			/* Create PDE entry */
	movl %ecx,(%edx)			/* Store identity PDE entry */
	movl %ecx,page_pde_offset(%edx)		/* Store kernel PDE entry */
	addl $4,%edx
	movl $1024, %ecx
11:
	stosl
	addl $0x1000,%eax
	loop 11b
	
/*
 * Enable paging
 */
	movl $swapper_pg_dir-__PAGE_OFFSET,%eax
	movl %eax,%cr3
/*	movl %cr0,%eax
	orl $0x80000000,%eax 
	movl %eax,%cr0*/
	
	pushl $0
	popfl

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
	
/*
 * Initialize eflags.  Some BIOS's leave bits like NT set.  This would
 * confuse the debugger if this code is traced.
 * XXX - best to initialize before switching to protected mode.
 */
	
.section .bss
.global swapper_pg_dir
swapper_pg_dir:
	.fill 1024,4,0
empty_zero_page:
	.fill 4096,1,0
idt_table:	.fill 256,8,0		/* idt is uninitialized */
	
.data
stack_start:
	.long __BOOT_DS

	.word 0
boot_gdt_descr:
	.word __BOOT_DS + 7
	.long boot_gdt_table - __PAGE_OFFSET
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
	
