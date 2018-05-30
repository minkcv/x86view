#include "interrupts.h"
#include <stdint.h>

bool interrupts_enabled()
{
    uint32_t flags;
    asm volatile ("pushf\npop %0" : "=g"(flags));
    return flags & 0x200;
}

