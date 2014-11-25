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

#---Module path-------
BOOT		= boot
#---------------------


boot/setup.img :
	($(CD) $(BOOT);$(MAKE))


default :
	$(MAKE) img

img :
	$(MAKE) $(BOOT)/setup.img

clean :
	$(SUDO) $(RM) *.img
	$(SUDO) $(RM) *.o

%.o  :  %.c Makefile
	$(CC) -c -o $*.o $*.c -O2 -Wall

