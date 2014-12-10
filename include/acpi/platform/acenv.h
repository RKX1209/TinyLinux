/*
 * include/acpi/platform/acenv.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ACPI_ACENV_H
#define _ACPI_ACENV_H

typedef char* va_list;

extern unsigned long acpi_native_int;

/* #define  _AUPBND                (sizeof (acpi_native_int) - 1) */
/* #define  _ADNBND                (sizeof (acpi_native_int) - 1) */

#define  _AUPBND                3
#define  _ADNBND                3

#define _bnd(X, bnd) (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)           (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_start(ap, A)         (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))

#endif
