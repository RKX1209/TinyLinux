/*
 * include/acpi/platform/acenv.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ACPI_ACENV_H
#define _ACPI_ACENV_H

typedef char* va_list;

#define _bnd(X, bnd) (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap,T) ( *(T *)( ((ap) += (_bnd(T,31))) - (_bnd(T,31)) ) )
#define va_start(ap,A) (void)( (ap) = (((char *) &(A)) + (_bnd(A,31))) )

#endif
