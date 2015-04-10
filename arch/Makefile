SRCS		= $(KERNEL)/head.S

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
MAKE		= make
#-------------------

CP		= cp
RM		= rm
CD		= cd
CAT		= cat
SUDO		= sudo

BOOT		= boot
KERNEL		= kernel

LD_SCRIPT_PATH 	= $(HOME_DIR)/ldscript
LD_SCRIPT	= $(LD_SCRIPT_PATH)/kernel.ld
KERNEL_ENTRY	= startup_32

arch.o : $(SRCS) Makefile
	$(CC) -m32 -nostdlib -o arch.o $(SRCS) $(CFLAGS)

default :
	$(MAKE) arch.o

clean :
	$(SUDO) $(RM) *.img

%.o  :  %.c Makefile
	$(CC) -m32 -c -o $*.o $*.c -O2 -Wall

%.o : %.asm Makefile
	$(NASM) -f elf32 -o $*.o $*.asm

