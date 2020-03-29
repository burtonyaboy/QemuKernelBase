#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "system.h"

void dump_regs(struct regs *r)
{
    terminal_writestring("DS: ");
    terminal_write_int(r->ds);
    terminal_writestring("\tES: ");
    terminal_write_int(r->es);
    terminal_writestring("\tFS: ");
    terminal_write_int(r->fs);
    terminal_writestring("\tGS: ");
    terminal_write_int(r->gs);
    terminal_writestring("\n");
    terminal_writestring("EDI: ");
    terminal_write_int(r->edi);
    terminal_writestring("\tESI: ");
    terminal_write_int(r->esi);
    terminal_writestring("\t");
    terminal_writestring("EBP: ");
    terminal_write_int(r->ebp);
    terminal_writestring("\t");
    terminal_writestring("ESP: ");
    terminal_write_int(r->esp);
    terminal_writestring("\n");
    terminal_writestring("EAX: ");
    terminal_write_int(r->eax);
    terminal_writestring("\tEBX: ");
    terminal_write_int(r->ebx);
    terminal_writestring("\tECX: ");
    terminal_write_int(r->ecx);
    terminal_writestring("\tEDX: ");
    terminal_write_int(r->edx);
    terminal_writestring("\n");
    terminal_writestring("INT NO: ");
    terminal_write_int((int)r->int_no);
    terminal_writestring("\tERR CODE: ");
    terminal_write_int((int)r->err_code);
    terminal_writestring("\n");
    terminal_writestring("EIP: ");
    terminal_write_int(r->eip);
    terminal_writestring("\tCS: ");
    terminal_write_int(r->cs);
    terminal_writestring("\tEFLAGS: ");
    terminal_write_int(r->eflags);
    terminal_writestring("\tUSERESP: ");
    terminal_write_int(r->useresp);
    terminal_writestring("\tSS: ");
    terminal_write_int(r->ss);
    terminal_writestring("\n");
}