/*
 * kernel/fork.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/fs_struct.h>
#include <abyon/mm.h>
#include <abyon/rbtree.h>
#include <abyon/sched.h>
#include <abyon/slab.h>

#include <asm/bitops.h>
#include <asm/cache.h>
#include <asm/current.h>
#include <asm/ptrace.h>
#include <asm/thread_info.h>

#define alloc_task_struct() kmem_cache_alloc(task_struct_cachep,0)
#define alloc_mm() kmem_cache_alloc(mm_cachep,0)
#define free_mm(mm)	(kmem_cache_free(mm_cachep, (mm)))

static kmem_cache_t *task_struct_cachep;
static kmem_cache_t *mm_cachep;
static kmem_cache_t *vm_area_cachep;
static kmem_cache_t *fs_cachep;

int max_threads;

void fork_init(unsigned long mempages){
  #define ARCH_MIN_TASKALIGN L1_CACHE_BYTES
  task_struct_cachep = kmem_cache_create("task_struct",sizeof(struct task_struct),
					 ARCH_MIN_TASKALIGN,0,0,0);
  max_threads = mempages / (8 * THREAD_SIZE / PAGE_SIZE);
  if(max_threads < 20) max_threads = 20;
  
}

static struct task_struct *dup_task_struct(struct task_struct *orig){
  struct task_struct *tsk = alloc_task_struct();
  struct thread_info *ti = alloc_thread_info(tsk);
  *ti = *orig->thread_info;
  *tsk = *orig;
  tsk->thread_info = ti;
  ti->task = tsk;
  return tsk;
}

static inline int mm_alloc_pgd(struct mm_struct *mm){
  mm->pgd = pgd_alloc(mm);
  return 0;
}


static struct mm_struct *mm_init(struct mm_struct *mm){
  mm_alloc_pgd(mm);
  return mm;
}

static inline int dup_mmap(struct mm_struct *mm, struct mm_struct *oldmm){
  mm->mmap = 0;
  mm->map_count = 0;
  mm->mm_rb = (struct rb_root){0};
  struct rb_node **rb_link = &mm->mm_rb.rb_node;
  struct rb_node *rb_parent = 0;
  struct vm_area_struct *mpnt;
  for(mpnt = current->mm->mmap; mpnt; mpnt = mpnt->vm_next){
    struct vm_area_struct *tmp = kmem_cache_alloc(vm_area_cachep,0);
    *tmp = *mpnt;
    tmp->vm_mm = mm;
    tmp->vm_next = 0;
    __vma_link_rb(mm,tmp,rb_link,rb_parent);
    rb_link = &tmp->vm_rb.rb_right;
    rb_parent = &tmp->vm_rb;
    mm->map_count++;
    copy_page_range(mm,current->mm,tmp);
  }
}

static int copy_mm(unsigned long clone_flags,struct task_struct *tsk){
  struct mm_struct *oldmm = current->mm;
  struct mm_struct *mm = alloc_mm();
  memcpy(mm,oldmm,sizeof(*mm));
  mm_init(mm);
  tsk->mm = mm;
  return 0;
}
static inline struct fs_struct *__copy_fs_struct(struct fs_struct *old){
  struct fs_struct *fs = kmem_cache_alloc(fs_cachep,0);
  if(fs){
    
  }
  return fs;
}

static inline int copy_fs(unsigned long clone_flags, struct task_struct *tsk){
  tsk->fs = __copy_fs_struct(current->fs);
  return 0;
}

void sched_fork(task_t *p){
  p->state = TASK_RUNNING;
  INIT_LIST_HEAD(&p->run_list);
  p->array = 0;
  p->time_slice = (current->time_slice + 1) >> 1;
  p->first_time_slice = 1;
  current->time_slice >>= 1;
  p->time_slice = sched_clock();
}

int copy_thread(int nr, unsigned long clone_flags, unsigned long esp,
		unsigned long unused,
		struct task_struct * p, struct pt_regs * regs){
  
}

static struct task_struct *copy_process(unsigned long clone_flags,
					unsigned long stack_start,
					struct pt_regs *regs,
					unsigned long stack_size,
					int *parent_tidptr,
					int *child_tidptr,				
					int pid){
  struct task_struct *p = dup_task_struct(current);
  copy_mm(clone_flags,p);
  copy_thread(0,clone_flags,stack_start,stack_size,p,regs);
  copy_fs(clone_flags,p);
  sched_fork(p);
  return p;  
}

long do_fork(unsigned long clone_flags,
	     unsigned long stack_start,
	     struct pt_regs *regs,
	     unsigned long stack_size,
	     int *parent_tidptr,
	     int *child_tidptr){
  
  long pid = alloc_pidmap();
  struct task_struct *p = copy_process(clone_flags,stack_start,regs,stack_size,
				       parent_tidptr,child_tidptr,pid);
  return pid;
}
