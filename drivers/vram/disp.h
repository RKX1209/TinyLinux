/*
 * drivers/vram/disp.h
 * Copyright (C) <2014>  <@RKX1209>
 */

#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

extern char* font0;
//char *font_dat = font0; /* font0 in ldscript */
void putfont(char *vram, int xsize, int x, int y, char col, char *font);
void putfonts_str(char *vram, int xsize, int x, int y, char col, unsigned char *s);
