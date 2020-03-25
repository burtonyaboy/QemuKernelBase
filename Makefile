OS_NAME=barebones
AS=i686-elf-as
CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -w
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

#I will clean this up *one day*
all:
	$(AS) boot.s -o boot.o
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o
	$(CC) $(CFLAGS) -c gdt.c -o gdt.o
	$(CC) $(CFLAGS) -c idt.c -o idt.o
	$(CC) $(CFLAGS) -c isrs.c -o isrs.o
	$(CC) -T linker.ld -o $(OS_NAME).bin -ffreestanding -O2 -nostdlib boot.o kernel.o gdt.o idt.o isrs.o -lgcc
	cp $(OS_NAME).bin isodir/boot/$(OS_NAME).bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(OS_NAME).iso isodir
run:
	qemu-system-i386 -cdrom $(OS_NAME).iso
clean:
	rm *.o $(OS_NAME).bin $(OS_NAME).iso
