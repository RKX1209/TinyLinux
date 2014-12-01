/*
 * include/asm/fixmap.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ASM_FIXMAP_H
#define _ASM_FIXMAP_H

#include <asm/page.h>

#define __FIXADDR_TOP	0xfffff000

enum fixed_addresses {
  FIX_HOLE,
  FIX_VSYSCALL,
  FIX_APIC_BASE,	/* local (CPU) APIC) -- required for SMP or not */
  FIX_IO_APIC_BASE_0,
  FIX_IO_APIC_BASE_END = FIX_IO_APIC_BASE_0,// + MAX_IO_APICS-1,
  FIX_CO_CPU,	/* Cobalt timer */
  FIX_CO_APIC,	/* Cobalt APIC Redirection Table */ 
  FIX_LI_PCIA,	/* Lithium PCI Bridge A */
  FIX_LI_PCIB,	/* Lithium PCI Bridge B */
  FIX_F00F_IDT,	/* Virtual mapping for IDT */
  FIX_CYCLONE_TIMER, /*cyclone timer register*/
  FIX_KMAP_BEGIN,	/* reserved pte's for temporary kernel mappings */
  FIX_KMAP_END,// = FIX_KMAP_BEGIN+(KM_TYPE_NR*NR_CPUS)-1,
  FIX_ACPI_BEGIN,
  FIX_ACPI_END,// = FIX_ACPI_BEGIN + FIX_ACPI_PAGES - 1,
  FIX_PCIE_MCFG,
  __end_of_permanent_fixed_addresses,
  /* temporary boot-time mappings, used before ioremap() is functional */
#define NR_FIX_BTMAPS	16
  FIX_BTMAP_END = __end_of_permanent_fixed_addresses,
  FIX_BTMAP_BEGIN = FIX_BTMAP_END + NR_FIX_BTMAPS - 1,
  FIX_WP_TEST,
  __end_of_fixed_addresses
};

#define FIXADDR_TOP	((unsigned long)__FIXADDR_TOP)

/* Exchange fix index and virtual address */
#define __fix_to_virt(x) (FIXADDR_TOP - ((x) << PAGE_SHIFT))
#define __virt_to_fix(x) ((FIXADDR_TOP - ((x) & PAGE_MASK)) >> PAGE_SHIFT)

#endif
