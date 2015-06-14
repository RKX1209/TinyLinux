/*
 * include/abyon/uio.h
 * Copyright (C) <2015>  <@RKX1209>
 */

#ifndef _ABYON_UIO_H
#define _ABYON_UIO_H

struct iovec{
  void *iov_base;
  unsigned long iov_len;
};

struct kiocb{
  
};

#define init_sync_kiocb(x,filp)			\
    do{ \
    struct task_struct *tsk = current; \    
    }while(0)

#endif
