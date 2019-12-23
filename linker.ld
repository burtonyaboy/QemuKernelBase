/* The bootloader will look at this image and start execution at this entry point*/
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final kernel image */
SECTIONS
{
	/* Begin putting sections at 1MiB, a conventional place for kernels to be loaded */
	. = 1M;

	/* put the multiboot header first */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
	/* read only data */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}
	/* r/w data initialized*/
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}
	/* better save space!!*/
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
