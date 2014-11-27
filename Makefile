IMGS		= $(BOOT)/loader.img $(KERNEL)/kernel.img
HOME_DIR	= "$(CURDIR)"
USER_NAME	= rkx

#---Build Command---
CC		= gcc
CFLAGS		= -fomit-frame-pointer -O2 -I$(HOME_DIR)/include -masm=intel -Wall
LFLAGS		= -T kernel_ld -Map kernel.map -nostdlib -e kernel_main --oformaty binary
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
WHOAMI		= whoami
GRUB_INSTALL	= grub-install

#---Module path-------
BOOT		= boot
KERNEL		= kernel
ARCH		= arch/i386
ARCH_BOOT	= $(ARCH)/boot
ARCH_KERNEL	= $(ARCH)/kernel

#---------------------

#---Virtual Loop Device---
SET_LPDEV	= losetup
LPDEV		= /dev/loop0
LPDEV_IMG	= loop_device.img
MNT_PATH	= /media/$(USER_NAME)
#-------------------------


MAIN_O		= $(KERNEL)/main.o
MAIN_C		= $(KERNEL)/main.c


$(BOOT)/loader.img : Makefile
	($(CD) $(BOOT);$(MAKE))

$(KERNEL)/kernel.img : Makefile
	($(CD) $(KERNEL);$(MAKE) img)

Abyon.img : $(IMGS) Makefile
	$(CAT) $(IMGS) > AbyonPlain.img
	$(DD) if=AbyonPlain.img of=Abyon.img conv=sync

default :
	$(MAKE) img

img :
	$(MAKE) Abyon.img

init :
	$(DD) if=/dev/zero of=$(LPDEV_IMG) bs=1k count=1440
	$(SUDO) $(SET_LPDEV) $(LPDEV) $(LPDEV_IMG)
	$(SUDO) mkdosfs $(LPDEV)
	$(SUDO) mount -t vfat $(LPDEV) $(MNT_PATH)

del :
	$(SUDO) umount $(LPDEV)
	$(SUDO) $(SET_LPDEV) -d $(LPDEV)

clean :
	$(SUDO) $(RM) *.img
	($(CD) $(BOOT);$(MAKE) clean)
	($(CD) $(KERNEL);$(MAKE) clean)

%.o  :  %.c Makefile
	$(CC) -c -o $*.o $*.c -O2 -Wall

