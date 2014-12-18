/*
 * include/asm/smp.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ASM_SMP_H
#define _ASM_SMP_H

#include <asm/current.h>
#define smp_processor_id() (current_thread_info()->cpu)

#endif
