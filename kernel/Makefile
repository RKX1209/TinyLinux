OBJS		= $(ARCH_KERNEL)/head.o main.o nasmfunc.o

HOME_DIR	= ..
USER_NAME	= rkx

#---Build Command---
CC		= gcc
CFLAGS		= -fomit-frame-pointer -I$(HOME_DIR)/include -Wall
LFLAGS		= -m elf_i386 --oformat binary -T $(LD_SCRIPT) -Map kernel.map -nostdlib -e $(KERNEL_ENTRY) -arch i386
LFLAGS2		= -m elf_i386 -T $(LD_SCRIPT) -Map kernel.map -nostdlib -e $(KERNEL_ENTRY) -arch i386
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

ARCH		= ../arch/i386
ARCH_BOOT	= $(ARCH)/boot
ARCH_KERNEL	= $(ARCH)/kernel

LD_SCRIPT_PATH 	= ../ldscript
LD_SCRIPT	= $(LD_SCRIPT_PATH)/kernel_ld
KERNEL_ENTRY	= startup_32

$(ARCH_KERNEL)/head.o : $(ARCH_KERNEL)/head.S Makefile
	$(CC) -o $(ARCH_KERNEL)/head.o $(ARCH_KERNEL)/head.S $(ASFLAGS)

kernel.img : $(OBJS) Makefile
	$(LD) $(LFLAGS) -o kernel.img $(OBJS)
	$(LD) $(LFLAGS2) -o kernel.elf $(OBJS)

default :
	$(MAKE) kernel.img

img :
	$(MAKE) kernel.img

clean :
	$(SUDO) $(RM) *.img

%.o  :  %.c Makefile
	$(CC) -m32 -c -o $*.o $*.c -O2 -Wall

%.o : %.asm Makefile
	$(NASM) -f elf32 -o $*.o $*.asm

