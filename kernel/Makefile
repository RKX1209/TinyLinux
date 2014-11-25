OBJS_PROTUNIX = Main.o

HOME_DIR	= "$(CURDIR)"
USER_NAME	= rkx

#---Build Command---
CC		= gcc
CFLAGS		= -fomit-frame-pointer -I$(HOME_DIR)/include -masm=intel -Wall
LFLAGS		= -m elf_i386 --oformat binary -T $(LD_SCRIPT) -Map kernel.map -nostdlib -e $(KERNEL_ENTRY) -arch i386
ASFLAGS		= -msyntax=intel -march=i686 --32
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

LD_SCRIPT_PATH 	= ../ldscript
LD_SCRIPT	= $(LD_SCRIPT_PATH)/kernel_ld
KERNEL_ENTRY	= kernel_main

kernel.img : main.o nasmfunc.o Makefile
	$(LD) $(LFLAGS) -o kernel.img main.o nasmfunc.o

default :
	$(MAKE) img

img :
	$(MAKE) kernel.img

clean :
	$(SUDO) $(RM) *.img
	$(SUDO) $(RM) *.o


%.o  :  %.c Makefile
	$(CC) -m32 -c -o $*.o $*.c -O2 -Wall

%.o : %.asm Makefile
	$(NASM) -f elf32 -o $*.o $*.asm

