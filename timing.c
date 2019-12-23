#include <stdbool.h>
#include "timing.h"

inline bool are_interrupts_enabled()
{
	unsigned long flags;
	asm volatile (  "pushf\n\t"
			"pop %0"
			: "=g"(flags) 
	);

	return flags & (1 << 9);
}


inline void enable_interrupts()
{
	if(are_interrupts_enabled())
	{
		return;
	}
	else
	{
		__asm__("sti\n\t");
	}
}


