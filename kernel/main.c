extern void io_hlt(void);
void kernel_main(void){
  int i;
  char *p;
  for(i = 0xa0000; i <= 0xbffff; i++){
    p = i;
    *p = i & 0x0f;
  }
  for(;;){
    io_hlt();
  }
  return;
}
