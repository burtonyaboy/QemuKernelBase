// all the colors we have?
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

struct regs
{
	unsigned int gs, fs, es, ds; 							// segs pushed last
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; 	// pushed by pusha
	unsigned char int_no, err_code;							// our dummy and isr 
	unsigned int eip, cs, eflags, useresp, ss;				// pushed by processor automatically
};

void debug(void);

size_t strlen(const char* str);

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);

static inline uint16_t vga_entry(unsigned char uc, uint8_t color);

void terminal_initialize(void); 

void terminal_setcolor(uint8_t color);

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

void memcpy(char *dest, char *src, unsigned int count);

void memsetw(unsigned short *dest, unsigned short character, unsigned int count);

void memset(char *dest, char character, unsigned int count);

void scroll(void);

void move_csr(void);

void cls();

void terminal_putchar(char c);

void terminal_write(const char* data, size_t size);

void terminal_writestring(const char* data);

unsigned char inportb(unsigned short _port);

void outportb(unsigned short _port, unsigned char _data);

void gdt_flush();

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

void gdt_install();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flag);

void idt_install();

void idt_load();

void fault_handler(struct regs *r);

void isrs_install(void);

void irq_install(void);

void irq_install_handler(int irq, void (*handler)(struct regs *r));

void irq_uninstall_handler(int irq);

void timer_install(void);

void timer_phase(int hz);

void dump_regs(struct regs *r);