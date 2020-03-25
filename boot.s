// declare constants for multiboot header
.set ALIGN,	1<<0	// align loaded modules on page boundaries
.set MEMINFO,	1<<1	// provide memory map
.set FLAGS, 	ALIGN | MEMINFO // multiboot flag field
.set MAGIC, 	0x1BADB002	// magic number that lets the bootloader find this
.set CHECKSUM,	-(MAGIC + FLAGS) // checksum of above to prove we are multiboot

// multiboot header
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

// allocate room for a small stack
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

// _start is the entry point, bootloader will jump to this location
.section .text
.global _start
.type _start, @function
_start:
	// set up the stack that is needed for c to function
	mov $stack_top, %esp
	// enable interrupts
	sti
	// ENTER THE KERNEL
	call kernel_main

	// k thats it now infinite loop :D
	cli
1:	hlt
	jmp 1b

.global gdt_flush
.type gdt_flush, @function
gdt_flush:
	lgdt (gp)
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	jmp %cs:flush_ret
flush_ret:
	ret

.size _start, . - _start
