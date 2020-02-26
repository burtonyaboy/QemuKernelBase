#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "system.h"

/* make sure cross compiler is being used */
#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif

// make sure we are compiling with a 32 bit compiler
#if !defined(__i386__)
#error "This must be built with a ix86-elf compiler"
#endif

size_t strlen(const char* str)
{
        size_t len = 0;
        while(str[len])
                len++;
        return len;
}

// upper bits are for background, lower bits for foreground
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

// The character and the color, when it's stored in the terminal buffer
// it stores each monospace as a color/ascii character
static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t term_y;
size_t term_x;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
	term_y = 0;
	term_x = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	//VGA text mode buffer
	terminal_buffer = (uint16_t*) 0xB8000;

	for(size_t y = 0; y < VGA_HEIGHT; y++){
		for(size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void memcpy(char *dest, char *src, unsigned int count)
{
	// change this to inline assembly function in the future 4 speed
	for(unsigned int i = 0; i < count; i++)
		dest[i] = src[i];
}

void memsetw(unsigned short *dest, unsigned short character, unsigned int count)
{
	for(unsigned int i = 0; i < count; i++)
		dest[i] = character;
}

void memset(char *dest, char character, unsigned int count)
{
	// change this to inline assembly function in the future 4 speed
	for(unsigned int i = 0; i < count; i++)
		dest[i] = character;
}

void scroll(void)
{	
	unsigned blank, temp;

	blank = vga_entry(' ', terminal_color);

	if(term_y >= VGA_HEIGHT)
	{
		//temp is usually equal to 1
		temp = term_y - VGA_HEIGHT + 1; 

		memcpy(
				(char *) terminal_buffer, // into terminal buffer
				(char *) terminal_buffer + temp * VGA_WIDTH*2, // from the 2nd line  
				(VGA_HEIGHT-temp) * (VGA_WIDTH * 2)  // whole buffer minus 1 line
			);
		
		memsetw(
				terminal_buffer + (VGA_HEIGHT - temp) * VGA_WIDTH, 
				blank, 
				VGA_WIDTH 
			);
		
		term_y = VGA_HEIGHT - 1;
	}
}

void move_csr(void)
{
	unsigned temp;

	temp = term_y * VGA_WIDTH + term_x;

	outportb(0x3D4, 14);
	outportb(0x3D5, temp >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, temp);
}

void cls()
{
	unsigned blank;
	int i;

	blank = vga_entry(' ', terminal_color);

	for(i = 0; i < 25; i++)
		memsetw(terminal_buffer + i * VGA_WIDTH, blank, VGA_WIDTH);

	term_y = 0;
	term_x = 0;
	move_csr();
}

void terminal_putchar(char c)
{

	if(c == '\n')
	{
		term_y++;
		term_x = 0;
	}
	else if(c == 0x08)
	{
		if(term_x != 0) term_x--;
	}
	else if(c == 0x09)
	{
		term_x = (term_x + 8) & ~(8 - 1);
	}
	else if(c == '\r')
	{
		term_x = 0;
	}
	else if( c >= ' ')
	{
		terminal_putentryat(c, terminal_color, term_x, term_y);
		term_x++;
	}

	if(term_x >= VGA_WIDTH)
	{
		term_x = 0;
		term_y++;
	}

	scroll();
	move_csr();
}

void terminal_write(const char* data, size_t size)
{
	for(size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void terminal_write_int(int n)
{
	int n_tmp = n;
	char out[11];

	for(int i = 0; i < 10; i++)
	{
		out[9-i] = (char) (n_tmp % 10) + 0x30;
		n_tmp /= 10;
	}

	out[10] = 0;

	terminal_writestring(out);
}

unsigned char inportb(unsigned short _port)
{
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
}

void outportb(unsigned short _port, unsigned char _data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void debug(void)
{
	terminal_putchar('X');
}

void kernel_main(void)
{
	//char int_enabled = 'T'; //are_interrupts_enabled() ? 'T' : 'F';
	// Initialize terminal interface
	terminal_initialize();
	gdt_install();
	// Newline support is left as an exercise
	char outstring[] = "Current Number: ";
	//char newbuff[17];

	//memcpy(newbuff, outstring, strlen(outstring));
	//terminal_writestring(newbuff);

	for(int i = 0; i < 27; i++)
	{
		terminal_writestring(outstring);
		terminal_write_int(i);
		terminal_putchar('\n');
	}
	
}
