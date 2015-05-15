/*
 * drivers/base/map.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/kobj_map.h>

struct kobj_map{
  struct probe{
    struct probe *next;
    unsigned long dev;
    unsigned long range;
    void *data;
  } *probes[255];  
};

struct kobj_map *kobj_map_init(kobj_probe_t *base_probe){
struct kobj_map *p = kmalloc(sizeof(struct kobj_map),0);
struct probe *base = kmalloc(sizeof(*base),0);
if((p == 0) || (base == 0)){
kfree(p);
kfree(base);
return 0;
}
base->dev = 1;
base->range = ~0;
//base->get = base_probe;
int i;
for(i = 0; i < 255; i++)
  p->probes[i] = base;
return p;
}
