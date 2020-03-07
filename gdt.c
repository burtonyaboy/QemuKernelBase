#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "system.h"

struct gdt_entry
{
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));

struct gdt_ptr
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_ptr gp;

extern void gdt_flush();

// Set up a descriptor in the gdt
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
	// set up descriptor base address
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	// set up descriptor limits
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);

	// Set up granularity and access flags
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}

void gdt_install()
{
	// set up gdt pointer and limit
	gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gp.base = &gdt;

	// null descriptor
	gdt_set_gate(0, 0, 0, 0, 0);

	// Code segment descriptor, base address 0, limit 4GB, 4KB gran, 32bit opcodes
	// and is CS descriptor
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

	// Data Segment, same as Code segment but descriptor says its a data segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	// Flush out old GDT and install changes
	gdt_flush();
}