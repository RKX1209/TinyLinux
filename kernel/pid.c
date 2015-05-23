/*
 * kernel/pid.c
 * Copyright (C) <2015>  <@RKX1209>
 */
#include <asm/bitops.h>
#include <asm/page.h>

#define BITS_PER_PAGE (PAGE_SIZE * 8)
#define PID_MAX_LIMIT 114514
#define RESERVED_PIDS 300
#define PIDMAP_ENTRIES ((PID_MAX_LIMIT + 8*PAGE_SIZE - 1)/PAGE_SIZE/8)
#define BITS_PER_PAGE_MASK (BITS_PER_PAGE-1)

#define mk_pid(map, off) ( ( (map) - pidmap_array)*BITS_PER_PAGE + (off))

typedef struct pidmap {
  int nr_free;
  void *page;
}pidmap_t;

static pidmap_t pidmap_array[PIDMAP_ENTRIES] = {[0 ... PIDMAP_ENTRIES - 1] = {BITS_PER_PAGE,0}};
int pid_max = 114514;
int last_pid;

int alloc_pidmap(void){
  int last = last_pid;
  int pid = last + 1;
  if(pid >= pid_max)
    pid = RESERVED_PIDS;
  int offset = pid & BITS_PER_PAGE_MASK;
  pidmap_t *map = &pidmap_array[pid/BITS_PER_PAGE];
  int max_scan = (pid_max + BITS_PER_PAGE - 1) / BITS_PER_PAGE - !offset;
  int i;
  for(i = 0; i <= max_scan; i++){
    if(!map->page){
      unsigned long page = get_zeroed_page(0);
      map->page = (void*)page;
    }
    if(map->nr_free){
      do{
	if(!test_and_set_bit(offset,map->page)){
	  map->nr_free--;
	  last_pid = pid;
	  return pid;
	}
	offset = find_next_zero_bit((map)->page,BITS_PER_PAGE,offset);
	pid = mk_pid(map,offset);
      }while(offset < BITS_PER_PAGE && pid < pid_max);
    }
    pid = mk_pid(map,offset);
  }
  return -1;
}
