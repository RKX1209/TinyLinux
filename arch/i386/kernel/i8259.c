/*
 * arch/i386/kernel/i8259.c
 * Copyright (C) <2014>  <@RKX1209>
 */
#include <abyon/irq.h>

#include <asm/hw_irq.h>
#include <asm/i8259.h>
#include <asm/io.h>
#include <asm/io_ports.h>

extern int printk(const char *fmt, ...);
extern void set_intr_gate(unsigned int n, void *addr);
extern void setup_pit_timer(void);
extern int setup_irq(unsigned int irq, struct irqaction *new);

int no_action(int cpl, void *dev_id, struct pt_regs *regs) { return 0; }
static struct irqaction irq2 = { no_action,0,"cascade",0,0};

unsigned int startup_8259A_irq(unsigned int irq){ 
  enable_8259A_irq(irq);
  return 0;
}

#define shutdown_8259A_irq disable_8259A_irq

void enable_8259A_irq(unsigned int irq){
}

void disable_8259A_irq(unsigned int irq){
}

static void mask_and_ack_8259A(unsigned int irq){ ; }

static void end_8259A_irq(unsigned int irq){
  if(!(irq_desc[irq].status & (IRQ_DISABLED | IRQ_INPROGRESS)) &&
     irq_desc[irq].action)
    enable_8259A_irq(irq);
}

static struct hw_interrupt_type i8259A_irq_type = {
  .typename = "XT-PIC",
  .startup = startup_8259A_irq,
  .shutdown = shutdown_8259A_irq,
  .enable = enable_8259A_irq,
  .disable = disable_8259A_irq,
  .ack = mask_and_ack_8259A,
  .end = end_8259A_irq,
};

void init_8259A(int auto_eoi){
  printk("Initializing PIC(8259A)...");
  outb(0xff,PIC_MASTER_IMR);
  outb(0xff,PIC_SLAVE_IMR);

  outb_p(0x11, PIC_MASTER_CMD);
  outb_p(0x20 + 0, PIC_MASTER_IMR);
  outb_p(1U << PIC_CASCADE_IR, PIC_MASTER_IMR);
  if (auto_eoi)
    outb_p(MASTER_ICW4_DEFAULT | PIC_ICW4_AEOI, PIC_MASTER_IMR);
  else
    outb_p(MASTER_ICW4_DEFAULT, PIC_MASTER_IMR);

  outb_p(0x11, PIC_SLAVE_CMD);
  outb_p(0x20 + 8, PIC_SLAVE_IMR);
  outb_p(PIC_CASCADE_IR, PIC_SLAVE_IMR);
  outb_p(SLAVE_ICW4_DEFAULT, PIC_SLAVE_IMR);
  if (auto_eoi)
    i8259A_irq_type.ack = disable_8259A_irq;
  else
    i8259A_irq_type.ack = mask_and_ack_8259A;
  //udelay(100);
  //outb(cached_master_mask, PIC_MASTER_IMR);
  //outb(cached_slave_mask, PIC_SLAVE_IMR);
  outb(0xfb, PIC_MASTER_IMR);
  outb(0xff, PIC_SLAVE_IMR);
  printk("Initializing PIC(8259A)... [OK]");
}

void init_ISA_irqs(void){
  init_8259A(0);
  unsigned long i;
  for(i = 0; i < NR_IRQS; i++){
    irq_desc[i].status = IRQ_DISABLED;
    irq_desc[i].action = 0;
    irq_desc[i].depth = 1;
    if(i < 16){
      irq_desc[i].handler = &i8259A_irq_type;
    }else{
      irq_desc[i].handler = &no_irq_type;
    }
  }
 
}

void interrupt_stub(){
  printk("interrupt happen");
  __asm__ __volatile__("iret");
}

void init_IRQ(void){
  printk("init_IRQ...");
  init_ISA_irqs();
  unsigned long i;
  for(i = 0; i < (NR_VECTORS - FIRST_EXTERNAL_VECTOR); i++){
    int vector = FIRST_EXTERNAL_VECTOR + i;
    if(i >= NR_IRQS) break;
    //interrupt[i] = interrupt_stub;
    printk("idt_table[%d](0x%x)->0x%x",vector,&interrupt[i],interrupt[i]);
    if(vector != SYSCALL_VECTOR) set_intr_gate(vector,interrupt[i]);
  }
  //printk("goto (0x%x)",interrupt[15]);
  //interrupt[15]();
  setup_irq(2,&irq2);
  setup_pit_timer();
  outb(0xf9, PIC_MASTER_IMR);
  outb(0xef, PIC_SLAVE_IMR);  
  printk("init_IRQ... [OK]");
}
