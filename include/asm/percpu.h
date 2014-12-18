/*
 * include/asm/percpu.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#ifndef _ASM_PERCPU_H
#define _ASM_PERCPU_H

#define DEFINE_PER_CPU(type,name) \
  __typeof__(type) per_cpu__##name
#define per_cpu(var,cpu) (*((void)(cpu),&per_cpu__##var))
#define __get_cpu_var(var) per_cpu__##var
#define DECLARE_PER_CPU(type, name) extern __typeof__(type) per_cpu__##name
#endif
