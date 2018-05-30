#include "keyboard.h"
#include "kernel/io.h"
#include "scancodes.h"
#include "interrupts.h"

char get_key()
{
    uint8_t scancode = 0;
    if (interrupts_enabled())
        scancode = get_key_intr();
    else
        scancode = get_key_nointr();
    return scancodes[scancode];
}

uint8_t get_key_nointr()
{
    while (!(inb(0x64) & 1));
    return inb(0x60);
}
uint8_t get_key_intr()
{
    // TODO
    return 0;
}

