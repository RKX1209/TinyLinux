/*
 * kernel/printk.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <acpi/platform/acenv.h>
#include <drivers/disp.h>

#define __LOG_BUF_LEN	(1 << 15)
#define __LOG_BUF_LIMIT 12

char __log_buf[__LOG_BUF_LEN][256];
static int log_buf_lim = __LOG_BUF_LIMIT;
static unsigned long logged_chars;

static unsigned long log_start;
static unsigned long log_end;

/* Get line number of idx */
#define LOG_BUF_MASK	(log_buf_len-1)
#define LOG_BUF(idx) (log_buf[(idx) & LOG_BUF_MASK])

extern int vscnprintf(char *buf, unsigned long size, const char *fmt, va_list args);

int vprintk(const char *fmt, va_list args){
  int printed_len = vscnprintf(__log_buf[log_end],sizeof(__log_buf[log_end]),fmt,args);

  char *vram = (char*)0xa0000;
  flush_screen();
  int y = 0,i = 0;
  int lines = (log_end - log_start + 1);
  if(lines > log_buf_lim) lines = log_buf_lim;
  
  for(i = 0; i < lines; i++){    
    putfonts_str(vram,320,0,y + i * 16,COL8_FFFFFF,
		 __log_buf[log_start + i],
		 (unsigned char *)&kernel_end);
  }
  log_end++;
  lines = (log_end - log_start + 1);
  if(lines > log_buf_lim) log_start++;

  return printed_len;
}
int printk(const char *fmt, ...){
  va_list args;
  va_start(args,fmt);
  int r = vprintk(fmt,args);
  //y += 16;
  return r;
}


