#include <system.h>

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