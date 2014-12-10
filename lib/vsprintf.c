/*
   lib/vsprintf.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <asm/string.h>
#include <asm-generic/div64.h>
#include <acpi/platform/acenv.h>

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define LARGE	64		/* use 'ABCDEF' instead of 'abcdef' */

/* num => buf */
static char* number(char * buf, char * end, unsigned long num, int base, int size){
  static const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  const char *digits = small_digits;
  char tmp[66];
  int i = 0;
  if(num == 0){
    tmp[i++] = '0';
  }
  else{
    while(num > 0){
      tmp[i++] = digits[num % base];
      num /= base;
    }
  }
  while(i-- > 0){
    if(buf <= end) *buf = tmp[i];
    ++buf;
  }
  return buf;
}

int vsnprintf(char *buf, unsigned long size, const char *fmt, va_list args){
  char *str = buf;
  char *end = buf + size - 1;
  int base = 10,len,i;
  char c;
  const char *s;
  unsigned int num;
  
  for(; *fmt; fmt++){
    /* Plain text */
    if(*fmt != '%'){
      if(str <= end) *str = *fmt;
      ++str;
      continue;
    }
    int flags = 0;
  repeat:;
    ++fmt;
    switch(*fmt){
    case '-': flags |= LEFT; goto repeat;
    case '+': flags |= PLUS; goto repeat;
    case ' ': flags |= SPACE; goto repeat;
    case '#': flags |= SPECIAL; goto repeat;
    case '0': flags |= ZEROPAD; goto repeat;      
    }
    
    switch(*fmt){
    case 'c':
      c = (char)va_arg(args,int);
      if(str <= end) *str = c;
      ++str;      
      continue;
    case 's':
      s = va_arg(args,char*);
      len = strnlen(s,-1);
      for(i = 0; i < len; i++){
	if(str <= end) *str = *s;
	++str;
      }
      continue;
    case 'x':
      base = 16;
      break;
    case 'd':
      break;
    default:
      if(str <= end) *str = '%';
      ++str;
      if(*fmt){
	if(str <= end) *str = *fmt;
	++str;
      }else{
	--fmt;
      }
      continue;
    }
    num = va_arg(args,unsigned int);
    str = number(str,end,num,base,size);
  }
  if(str <= end) *str = '\0';
  else if(size > 0) *end = '\0';
  return str - buf;
}

int vscnprintf(char *buf, unsigned long size, const char *fmt, va_list args){
  int i = vsnprintf(buf,size,fmt,args);
  return (i >= size) ? (size - 1) : i;
}
