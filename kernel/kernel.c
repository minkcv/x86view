#include "../drivers/vga_screen.h"
#include "memory.h"
#include <stdint.h>

void kernel_main()
{
    uint8_t* vidmem = (uint8_t*)0xB8000;
    memset(vidmem + sizeof(uint8_t), 0xF << 4, 1);
    memset(vidmem, 'X', 1);
    //*vidmem = 0xF << 4;
    //*vidmem = 'X';
}

