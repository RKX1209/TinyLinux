#include <drivers/disp.h>
//0x102c14
extern void io_hlt(void);
extern int printk(const char *fmt, ...);
extern void setup_arch();
extern void kernel_main(void);
void kernel_main(void){
  int i;
  unsigned char* font0 = (char*)(kernel_end - __PAGE_OFFSET);
  char *p = font0;
  for(i = 0xa0000; i <= 0xaffff; i++){
    p = (char *)i;
    *p = COL8_000000;
  }
  int years = 20;
  printk("I am 0x%x years old",years);
  setup_arch();
  printk("Setuped arch");
  //putfonts_str(vram,320,30,10,COL8_FFFFFF,test,&kernel_end);
  for(;;){
    io_hlt();
  }
  return;
}
