OS_NAME=barebones
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
CC=i686-elf-gcc
AA=i686-elf-as
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

.c.o:
	$(CC) $(CFLAGS) -c $<

.s.o:
	$(AA) $<

iso:
	cp $(OS_NAME).bin isodir/boot/$(OS_NAME).bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(OS_NAME).iso isodir

all:
	$(CC) -T linker.ld -o $(OS_NAME).bin $@.0 $(LDFLAGS)

run:
	qemu-system-i386 -cdrom $(OS_NAME).iso

clean:
	rm *.o $(OS_NAME).bin $(OS_NAME).iso
