OS_NAME=barebones
AS=i686-elf-as
CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc


all:
	$(AS) boot.s -o boot.o
	$(CC) $(CFLAGS) -c kstring.c -o kstring.o
	$(CC) $(CFLAGS) -c timing.c -o timing.o
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o
	$(CC) -T linker.ld -o $(OS_NAME).bin -ffreestanding -O2 -nostdlib boot.o kernel.o kstring.o timing.o -lgcc
	cp $(OS_NAME).bin isodir/boot/$(OS_NAME).bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(OS_NAME).iso isodir
run:
	qemu-system-i386 -cdrom $(OS_NAME).iso
clean:
	rm boot.o kernel.o $(OS_NAME).bin $(OS_NAME).iso
