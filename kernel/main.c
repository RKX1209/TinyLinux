#include <drivers/disp.h>
//0x102c14
extern void io_hlt(void);

extern int printk(const char *fmt, ...);
extern void setup_arch();
extern void kernel_main(void);
extern int _text_start;
extern int _text_end;
extern int _data_start;
extern int _data_end;
extern int _bss_start;
extern int _bss_end;
void kernel_main(void){
  int i;
  char *p;
  for(i = 0xa0000; i <= 0xaffff; i++){
    p = (char *)i;
    *p = COL8_000000;
  }
  printk("Welcome to Abyon kernel!");
  
  //0x10105e 0x10598c
  setup_arch();
  /* printk("text:[0x%x,0x%x]",&(_text_start),&(_text_end)); */
  /* printk("data:[0x%x,0x%x]",&(_data_start),&(_data_end)); */
  /* printk("bss:[0x%x,0x%x]",&(_bss_start),&(_bss_end)); */
  /* printk("pagetable:[0x%x]",swapper_pg_dir); */
  /* printk("font:[0x%x]",&kernel_end); */
  printk("Enter sleeping...");
  for(;;){
    io_hlt();
  }
  return;
}
