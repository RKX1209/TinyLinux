/*
 * security/seabyon/avc.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/list.h>
#include <abyon/security.h>

#define AVC_CACHE_SLOTS 512
struct av_decision{
  unsigned int allowed;
  unsigned int decided;
  unsigned int auditallow;
  unsigned int auditdeny;
  unsigned int seqno;
};

struct avc_entry{
  unsigned int ssid;
  unsigned int tsid;
  unsigned int tclass;
  struct av_decision avd;
};

struct avc_node{
  struct avc_entry ae;
  struct list_head list;
};

struct avc_cache{
  struct list_head slots[AVC_CACHE_SLOTS];
  
};

static struct avc_cache avc_cache;

void avc_init(void){
  int i;
  for(i = 0; i < AVC_CACHE_SLOTS; i++){
    INIT_LIST_HEAD(&avc_cache.slots[i]);
  }
}

static inline int avc_hash(unsigned long ssid, unsigned long tsid,
			   unsigned short tclass){
  return (ssid ^ (tsid<<2) ^ (tclass<<4)) & (AVC_CACHE_SLOTS - 1);
}

static inline struct avc_node *avc_search_node(unsigned int ssid,unsigned int tsid,unsigned short tclass){
  int hvalue = avc_hash(ssid,tsid,tclass);
  struct avc_node *ret = &avc_cache.slots[hvalue];
  return ret;
}
static struct avc_node *avc_lookup(unsigned int ssid,unsigned int tsid,
				   unsigned short tclass,unsigned int requested){
  struct avc_node *node = avc_search_node(ssid,tsid,tclass);  
  return node;
}

int avc_has_perm(unsigned long ssid,unsigned long tsid,unsigned short tclass,
		   unsigned long requested,int dumm){
  struct avc_node *node = avc_lookup(ssid,tsid,tclass,requested);
  struct avc_entry *p_ae = &node->ae;
  unsigned int denied = requested & ~(p_ae->avd.allowed);
  if(denied) return -1;
  return 1;
}
		   
