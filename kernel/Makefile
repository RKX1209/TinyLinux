 OBJS		= $(ARCH_KERNEL)/head.o $(KERNEL)/main.o $(KERNEL)/gasfunc\
		  $(KERNEL)/sched.o $(KERNEL)/printk.o \
		  $(KERNEL_IRQ)/manage.o \
		  $(ARCH_KERNEL)/setup.o $(ARCH_KERNEL)/traps.o $(ARCH_KERNEL)/entry.o \
		  $(ARCH_KERNEL)/init_task.o $(ARCH_KERNEL)/i8259.o $(ARCH_KERNEL)/irq.o \
		  $(ARCH_KERNEL_CPU)/common.o \
		  $(ARCH_KERNEL_TIMERS)/timer_pit.o \
		  $(ARCH_MM)/init.o \
		  $(ARCH_LIB)/memcpy.o $(ARCH_LIB)/bitops.o \
		  $(MM)/bootmem.o $(MM)/page_alloc.o \
		  $(LIB)/vsprintf.o \
		  $(DRIVERS_VRAM)/disp.o \
		  $(ARCH_KERNEL)/tail.o

HOME_DIR	= ..

#---Build Command---
CC		= gcc
CFLAGS		= -I$(HOME_DIR)/include -Wall -fno-tree-scev-cprop -Wno-pointer-sign -fno-stack-protector
LFLAGS		= -m elf_i386 --oformat binary -T $(LD_SCRIPT) -Map kernel.map -nostdlib -e $(KERNEL_ENTRY) -arch i386
LFLAGS2		= -m elf_i386 -T $(LD_SCRIPT) -Map kernel_e.map -nostdlib -e $(KERNEL_ENTRY) -arch i386
ASFLAGS		= -c -m32 -I$(HOME_DIR)/include
NASM		= nasm
LD		= ld
DD		= dd
MAKE		= make
#-------------------

CP		= cp
RM		= rm
CD		= cd
CAT		= cat
SUDO		= sudo

#---Kernel path-------------
KERNEL		= .
KERNEL_IRQ	= $(KERNEL)/irq
#-------------------------

#---Arch path-------------
ARCH			= ../arch
ARCH_BOOT		= $(ARCH)/boot
ARCH_KERNEL		= $(ARCH)/kernel
ARCH_KERNEL_CPU		= $(ARCH_KERNEL)/cpu
ARCH_KERNEL_TIMERS 	= $(ARCH_KERNEL)/timers
ARCH_MM			= $(ARCH)/mm
ARCH_LIB		= $(ARCH)/lib
#-------------------------

#---MM path-------------
MM		= ../mm
#-------------------------

#---Lib path-------------
DRIVERS		= ../drivers
DRIVERS_VRAM	= $(DRIVERS)/vram
#-------------------------

#---Lib path-------------
LIB		= ../lib
#-------------------------


LD_SCRIPT_PATH 	= ../ldscript
LD_SCRIPT	= $(LD_SCRIPT_PATH)/kernel.ld
KERNEL_ENTRY	= startup_32

kernel.img : $(OBJS) Makefile
	$(LD) $(LFLAGS) -o kernel.img $(OBJS)
	$(LD) $(LFLAGS2) -o kernel.elf $(OBJS)
	$(CAT) kernel.img > kernel_plain.img
	#$(CAT) kernel_plain.img $(BSS_AREA) > kernel.img
default :
	$(MAKE) kernel.img

img :
	$(MAKE) kernel.img


%.o  :  %.c Makefile
	$(CC) -m32 -c -o $*.o $*.c $(CFLAGS)

%.o :	%.S Makefile
	$(CC) -o $*.o $*.S $(ASFLAGS)

$(KERNEL_IRQ)/%.o : $(KERNEL_IRQ)/%.c Makefile
	$(CC) -m32 -c -o $(KERNEL_IRQ)/$*.o $(KERNEL_IRQ)/$*.c $(CFLAGS)

$(KERNEL_IRQ)/%.o : $(KERNEL_IRQ)/%.S Makefile
	$(CC) -o $(KERNEL_IRQ)/$*.o $(KERNEL_IRQ)/$*.S $(ASFLAGS)

$(ARCH_KERNEL_CPU)/%.o : $(ARCH_KERNEL_CPU)/%.c Makefile
	$(CC) -m32 -c -o $(ARCH_KERNEL_CPU)/$*.o $(ARCH_KERNEL_CPU)/$*.c $(CFLAGS)

$(ARCH_KERNEL_CPU)/%.o : $(ARCH_KERNEL_CPU)/%.S Makefile
	$(CC) -o $(ARCH_KERNEL_CPU)/$*.o $(ARCH_KERNEL_CPU)/$*.S $(ASFLAGS)

$(ARCH_KERNEL)/%.o : $(ARCH_KERNEL)/%.c Makefile
	$(CC) -m32 -c -o $(ARCH_KERNEL)/$*.o $(ARCH_KERNEL)/$*.c $(CFLAGS)

$(ARCH_KERNEL)/%.o : $(ARCH_KERNEL)/%.S Makefile
	$(CC) -o $(ARCH_KERNEL)/$*.o $(ARCH_KERNEL)/$*.S $(ASFLAGS)

$(ARCH_MM)/%.o : $(ARCH_MM)/%.c Makefile
	$(CC) -m32 -c -o $(ARCH_MM)/$*.o $(ARCH_MM)/$*.c $(CFLAGS)

$(ARCH_MM)/%.o : $(ARCH_MM)/%.S Makefile
	$(CC) -o $(ARCH_MM)/$*.o $(ARCH_MM)/$*.S $(ASFLAGS)

$(MM)/%.o : $(MM)/%.c Makefile
	$(CC) -m32 -c -o $(MM)/$*.o $(MM)/$*.c $(CFLAGS)

$(MM)/%.o : $(MM)/%.S Makefile
	$(CC) -o $(MM)/$*.o $(MM)/$*.S $(ASFLAGS)

$(LIB)/%.o : $(LIB)/%.c Makefile
	$(CC) -m32 -c -o $(LIB)/$*.o $(LIB)/$*.c $(CFLAGS)

$(LIB)/%.o : $(LIB)/%.S Makefile
	$(CC) -o $(LIB)/$*.o $(LIB)/$*.S $(ASFLAGS)




