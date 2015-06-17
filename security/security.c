/*
 * security/security.c
 * Copyright (C) <2015>  <@RKX1209>
 */

#include <abyon/security.h>

struct security_operations dummy_security_ops;
struct security_operations *security_ops;

int security_init(void){
  security_ops = &dummy_security_ops;
}

int register_security(struct security_operations *ops){
  security_ops = ops;
  return 0;
}

int unregister_security(struct security_operations *ops){
  security_ops = &dummy_security_ops;
  return 0;
}
