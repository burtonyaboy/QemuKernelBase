#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "system.h"

int timer_ticks = 0;

void timer_phase(int hz)
{
    int divisor = 1193180 / hz;
    outportb(0x43, 0x36);
    outportb(0x40, divisor & 0xFF);
    outportb(0x40, divisor >> 8);
}

// handle the timer tick
void timer_handler(struct regs *r)
{
    //increment tick count
    timer_ticks++;

    //Ever 18 clock cycles (1 second) print a message
    if(timer_ticks % 18 == 0)
    {
        terminal_writestring("One second has passed\n");
    }
}

void timer_install(void)
{
    struct regs *r;
    //install timer handle to irq0
    irq_install_handler(0, timer_handler);
}