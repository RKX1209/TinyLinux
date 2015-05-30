/*
 * kernel/sys.c
 * Copyright (C) <2015>  <@RKX1209>
 */
#include <abyon/sched.h>

#include <asm/current.h>

static int groups_search(struct group_info *group_info, unsigned int grp){
  int left = 0;
  int right = group_info->ngroups;
  while(left < right){
    int mid = (left + right) / 2;
    int cmp = grp - GROUP_AT(group_info,mid);
    if(cmp > 0) left = mid + 1;
    else if(cmp < 0) right = mid;
    else return 1;
  }
  return 0;
}

int in_group_p(unsigned long grp){
  int retval = 1;
  if(grp != current->fsgid){
    retval = groups_search(current->group_info,grp);
  }
  return retval;
}
