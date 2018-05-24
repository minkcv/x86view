#include <stdint.h>

void kernel_main()
{
    uint8_t* vidmem = (uint8_t*)0xB8000;
    *vidmem = 0xF << 4;
    *vidmem = 'X';
}
