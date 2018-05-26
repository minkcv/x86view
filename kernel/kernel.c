#include "drivers/vga_screen.h"
#include "kernel/memory.h"
#include <stdint.h>

void kernel_main()
{
    uint8_t* vidmem = (uint8_t*)0xB8000;
    memset(vidmem + 1, 0x0F, 1);
    memset(vidmem, 'X', 1);
}

