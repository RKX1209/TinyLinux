# OBJS_PROTUNIX = Main.o ProcMng.o System1.o
OBJS_PROTUNIX = Main.o

HOME_DIR	= "$(CURDIR)"
USER_NAME	= rkx

CC		= gcc
CFLAGS		= -fomit-frame-pointer -O2 -I$(HOME_DIR)/include -masm=intel -Wall
ASFLAGS		= -msyntax=intel -march=i686 --32
LFLAGS		= -T linkerscript -Map kernel.map -nostdlib -e _kernel_entry --oformaty binary
NASM		= nasm
LD		= ld
DD		= dd
CP		= cp
RM		= rm
CD		= cd
CAT		= cat
MAKE		= make
SUDO		= sudo
WHOAMI		= whoami
GRUB_INSTALL	= grub-install


default :
	$(MAKE) loader.img

img :
	$(MAKE) loader.img

clean :
	$(SUDO) $(RM) *.img
	$(SUDO) $(RM) *.o

%.o  :  %.c Makefile
	$(CC) -c -o $*.o $*.c -O2 -Wall


loaderS1.img : loaderS1.asm Makefile
	$(NASM) -o loaderS1.img loaderS1.asm

loaderS2.img : loaderS2.asm Makefile
	$(NASM) -o loaderS2.img loaderS2.asm

loader.img : loaderS1.img loaderS2.img Makefile
	$(CAT) loaderS1.img loaderS2.img > loader.img
