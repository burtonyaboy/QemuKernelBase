#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "system.h"

struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;         //kernel segment goes here
    unsigned char always0;      //always set to zero
    unsigned char flags;        //available y/n, rings, the number 14...
    unsigned short base_hi;     
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// our idt will have 256 entries
// 32 will be defined for now
// the rest are a trap for now
struct idt_entry idt[256];
struct idt_ptr idtp;

// load the idt, defined in boot.s
extern void idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flag)
{
    idt[num].base_hi =  (base >> 16) & 0xFFFF;
    idt[num].base_lo =  base & 0xFFFF;
    idt[num].always0 = 0x0;
    idt[num].flags  = flag;
    idt[num].sel    = sel;
}

void idt_install()
{
    // setup the idt pointer
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    // set isr's here using idt_set_gate

    idt_load();
}