/*
 * drivers/vram/disp.c
 * Copyright (C) <2014>  <@RKX1209>
 */

#include <drivers/disp.h>

void putfont(char *vram, int xsize, int x, int y, char col, unsigned char *font){
  
  int i,j;
  int ysize = 16;
  for(i = 0; i < ysize; i++){
    char *p = vram + (y + i) * xsize + x;
    char d = font[i];
    for(j = 0; j < 8; j++){
      if(d & 0x01)
  	p[7 - j] = col;
      d >>= 1;
    }
  }
  return;
}

/* Put string with font(16 x 8) */
/* char font_A[16] = { */
/*   0x00, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24, */
/*   0x24, 0x7e, 0x42, 0x42, 0x42, 0xe7, 0x00, 0x00 */
/* }; */
void flush_screen(){
  char *p;
  unsigned long i;
  for(i = 0xa0000; i <= 0xaffff; i++){
    p = (char *)i;
    *p = COL8_000000;
  }
}

void putfonts_str(char *vram, int xsize, int x, int y, char col, unsigned char *s, unsigned char *k){
  int i = 0;
  unsigned char* font0 = k;
  
  for (i = 0; *s; s++,i++) {
    if(*s == '\n'){
      y += 16;
      x = 0;
    }else{
      putfont(vram, xsize, x, y, col, (unsigned char*)(font0 + *s * 16));
      x += 8;
    }
  }
  return;
}
