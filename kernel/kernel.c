#include "drivers/vga_console.h"
#include "drivers/keyboard.h"
#include "memory.h"
#include "io.h"
#include <stdbool.h>
#include <stdint.h>


bool a20Enabled();

void kernel_main()
{
    while (true)
    {
        uint8_t k = get_keycode();
        char c = get_char(k);
        if (c != '\0')
        {
            print_char(c);
            //print_byte_hex((c >> 4) & 0xF);
            //print_byte_hex(c & 0xF);
        }
    }
    /*
    uint8_t* memory = (uint8_t*)0x100000;
    int i;
    for (i = 0; i < (80 * 25) / 2; i++)
    {
        uint8_t byte = *(memory + i);
        print_byte_hex((byte >> 4) & 0xF);
        print_byte_hex(byte & 0xF);
    }
    */

    //print_string("Hello world!\n");
    //print_string("welcome to the danger zone");

    //bool enabled = a20Enabled();
    //if (enabled)
        //print('E');
    //else
        //print('D');
}

bool a20Enabled()
{
    // 1 Megabyte is 1024 bytes * 1024 bytes
    // 1024 * 1024 = 1048576 = 0x100000
    uint8_t* odd  = (uint8_t*)0x112345;
    uint8_t* even = (uint8_t*)0x012345;
    *odd = 0x1;
    *even = 0x0;
    print_byte_hex(*odd);
    print_byte_hex(*even);
    return *odd != *even;
}

