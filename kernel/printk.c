/*
 * kernel/printk.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <acpi/platform/acenv.h>
#include <drivers/disp.h>

#define __LOG_BUF_LEN	(1 << 24)

static char __log_buf[__LOG_BUF_LEN];
static char *log_buf = __log_buf;
static int log_buf_len = __LOG_BUF_LEN;
static unsigned long logged_chars;

static unsigned long log_start;
static unsigned long con_start;
static unsigned long log_end;
static int x = 0;
static int y = 0;
/* Get line number of idx */
#define LOG_BUF_MASK	(log_buf_len-1)
#define LOG_BUF(idx) (log_buf[(idx) & LOG_BUF_MASK])

int vsnprintf(char *buf, unsigned long size, const char *fmt, va_list args);

static void emit_log_char(char c){
  LOG_BUF(log_end) = c;
  log_end++;
  if(log_end - log_start > log_buf_len)
    log_start = log_end - log_buf_len;
  if (log_end - con_start > log_buf_len)
    con_start = log_end - log_buf_len;
  if(logged_chars < log_buf_len)
    logged_chars++;
  
}

int vprintk(const char *fmt, va_list args){
  char printk_buf[1024];
  int printed_len = vscnprintf(printk_buf,sizeof(printk_buf),fmt,args);
  char *vram = (char*)0xa0000;  
  putfonts_str(vram,320,x,y,COL8_FFFFFF,printk_buf,&kernel_end);  
  char *p;
  static int new_line_text;
  /* for(p = printk_buf; *p; p++){ */
  /*   if(new_line_text) */
  /*     emit_log_char('>'); */
  /*   new_line_text = 0; */
  /*   emit_log_char(*p); */
  /*   if(*p == '\n') new_line_text = 1; */
  /* } */
  //putfonts_str(vram,320,30,10,COL8_FFFFFF,fmt,&kernel_end);
  return printed_len;
}
int printk(const char *fmt, ...){
  char *vram = (char*)0xa0000;  
  //putfonts_str(vram,320,x,y,COL8_FFFFFF,fmt,&kernel_end);
  va_list args;
  va_start(args,fmt);
  int r = vprintk(fmt,args);
  y += 16;
  return r;
}


