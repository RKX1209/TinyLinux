/*
 * include/abyon/sched.h
 * Copyright (C) <2014>  <@RKX1209>
 */
#ifndef _ABYON_SCHED_H
#define _ABYON_SCHED_H

#include <abyon/list.h>
#include <abyon/rbtree.h>
#include <abyon/signal.h>
#include <abyon/thread_info.h>

#include <asm/current.h>
#include <asm/pgtable.h>
#include <asm/processor.h>

#define MAX_USER_RT_PRIO	100
#define MAX_RT_PRIO		MAX_USER_RT_PRIO

#define MAX_PRIO		(MAX_RT_PRIO + 40)

#define TASK_RUNNING		0
#define TASK_INTERRUPTIBLE	1
#define TASK_UNINTERRUPTIBLE	2
#define TASK_STOPPED		4
#define TASK_TRACED		8
/* in tsk->exit_state */
#define EXIT_ZOMBIE		16
#define EXIT_DEAD		32

#define SCHED_NORMAL		0
#define SCHED_FIFO		1
#define SCHED_RR		2
#define BITMAP_SIZE ((((MAX_PRIO + 8) / 8) + sizeof(long) - 1) / sizeof(long))

#define NGROUPS_PER_BLOCK	((int)(PAGE_SIZE / sizeof(unsigned int)))

struct group_info{
  int ngroups;
  int nblocks;
  unsigned int *blocks[0];
};

#define GROUP_AT(gi,i) \
  ( (gi)->blocks[(i)/NGROUPS_PER_BLOCK][(i) % NGROUPS_PER_BLOCK])

typedef struct task_struct task_t;

extern void sched_init(void);
extern void init_idle(task_t *idle, int cpu);

typedef struct prio_array {
  unsigned long nr_active;
  unsigned long bitmap[BITMAP_SIZE];
  struct list_head queue[MAX_PRIO];
} prio_array_t;

typedef struct runqueue{
  unsigned long nr_runnning;
  prio_array_t *active,*expired,arrays[2];
  task_t *curr,*idle;
  int best_expired_prio;
  unsigned long nr_switches;
}runqueue_t;

struct mm_struct{
  struct vm_area_struct *mmap;
  struct rb_root mm_rb;
  unsigned long mm_count;
  unsigned long map_count;
  pgd_t *pgd;
};
struct namespace;
struct task_struct{
  long state;
  struct thread_info *thread_info;
  unsigned long flags;
  unsigned long ptrace;
  int prio,static_prio;
  struct list_head run_list;
  prio_array_t *array;
  unsigned long policy;
  struct mm_struct *mm,*active_mm;
  struct thread_struct thread;
  struct namespace *namespace;
  struct task_struct *next_task;
  unsigned long long timestamp;
  unsigned int time_slice;
  unsigned int first_time_slice;
  unsigned long uid,euid,suid,fsuid;
  unsigned long gid,egid,sgid,fsgid;
  struct fs_struct *fs;
  int activated;
  struct group_info* group_info;
  struct file_struct *files;
  void *security;
};

static inline void set_task_cpu(struct task_struct *p, unsigned int cpu){
  p->thread_info->cpu = cpu;
}

#endif
