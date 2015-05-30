/*
 * kernel/main.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <abyon/sched.h>
#include <asm/system.h>
#include <asm/unistd.h>
#include <drivers/disp.h>

extern void io_hlt(void);

extern int printk(const char *fmt, ...);
extern void setup_arch();
extern void trap_init();
extern void init_IRQ();
extern void init_timers();
extern void softirq_init();
extern void key_int();
extern void pgtable_cache_init(void);
extern void kernel_main(void);
static void rest_init(void);
static int init(void *unused);

static char *ramdisk_execute_command = "/init";
static const char *argv_init[3] = {"init",0,};
const char *envp_init[4] = {"HOME=/","TERM=linux",0,};

void kernel_main(void){
  printk("Welcome to Abyon kernel!");
  setup_arch();//init e820, bootmem, pagetable(pgd,pte...), fix area  paging on
  
  sched_init(); //init runqueue, current <= idle_proc

  trap_init(); //init trap tables

  init_IRQ(); //init 8259A, IQR table[i] = interrupt[i] => do_IRQ(entry.S), init PIT

  init_timers();//init softirq[TIMER_SOFTIRQ] => timer

  softirq_init();//init softirq[TASKLET,HI_SOFTIRQ]

  local_irq_enable();
  
  key_int();

  vfs_caches_init_early();//init dentry cache, init inode cache

  //mem_init(); //free bootmem area
  //kmem_cache_init(); //init slab allocator
  
  pgtable_cache_init();

  vfs_caches_init();

  rest_init();
  printk("Enter sleeping...");
  for(;;){
    io_hlt();
  }
  return;
}

static void rest_init(void){
  kernel_thread(init,0,0);
  schedule();
  cpu_idle();
}

static void do_basic_setup(void){
  
}

int kernel_execve(const char *filename,
		  const char *const argv[],
		  const char *const envp[]){
  long __res;
  __asm__ __volatile__("int $0x80" \
		       : "=a" (__res) \
		       : "" (__NR_execve), "b" (filename), "c" (argv), "d" (envp) :"memory");
  return __res;
}

static void run_init_process(const char *init_filename){
  argv_init[0] = init_filename;
  kernel_execve(init_filename,argv_init,envp_init);
}

static int init(void *unused){
  populate_rootfs();
  do_basic_setup();
  if(ramdisk_execute_command){
    run_init_process(ramdisk_execute_command);
  }
  return 0;
}
