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
void putfonts_str(char *vram, int xsize, int x, int y, char col, unsigned char *s, unsigned char *k){
  //unsigned char *s2 = (unsigned char*)(s - __PAGE_OFFSET);
  unsigned char *s2 = s;
  //unsigned char *fA = (unsigned char*)(font_A - __PAGE_OFFSET);
  int sx = x;
  int i = 0;
  //unsigned char* font0 = (unsigned char*)(k - __PAGE_OFFSET);
  unsigned char* font0 = k;
  for (i = 0; *s2 != 0x00; s2++,i++) {
    putfont(vram, xsize, x, y, col, (unsigned char*)(font0 + *s2 * 16));
    x += 8;
  }
  return;
}
