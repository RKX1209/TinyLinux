extern void io_hlt(void);
extern void kernel_main(void);
int bss;
void kernel_main(void){
  int i;
  char *p;
  for(i = 0xa0000; i <= 0xaffff; i++){
    p = i;
    *p = i & 0x0f;
    bss = i;
  }
  for(;;){
    io_hlt();
  }
  return;
}
