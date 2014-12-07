 OBJS		= $(ARCH_KERNEL)/head.o main.o gasfunc.o $(ARCH_KERNEL)/setup.o\
		  $(ARCH_MM)/init.o $(MM)/bootmem.o $(ARCH_LIB)/memcpy.o

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

#---Arch path-------------
ARCH		= ../arch
ARCH_BOOT	= $(ARCH)/boot
ARCH_KERNEL	= $(ARCH)/kernel
ARCH_MM		= $(ARCH)/mm
ARCH_LIB	= $(ARCH)/lib
#-------------------------

#---MM path-------------
MM		= ../mm
#-------------------------


LD_SCRIPT_PATH 	= ../ldscript
LD_SCRIPT	= $(LD_SCRIPT_PATH)/kernel.ld
KERNEL_ENTRY	= startup_32


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
	$(CC) -m32 -c -o $*.o $*.c $(CFLAGS)

%.o :	%.S Makefile
	$(CC) -o $*.o $*.S $(ASFLAGS)

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




