#include "prompt.h"
#include "string.h"
#include "memory.h"
#include "drivers/scancodes.h"
#include "drivers/keyboard.h"
#include "drivers/vga_console.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


void prompt_run()
{
    char* buffer = (char*)0x7E00; // TODO where to put this in memory?
    size_t length = 0;
    print_prompt();
    while (true)
    {
        uint8_t k = get_keycode();
        if (k == KEY_BACKSPACE && length > 0)
        {
            length--;
            buffer[length] = '\0';
            console_backspace();
        }
        else if (k == KEY_ENTER)
        {
            parse_command(buffer);
            memset(buffer, 0, length);
            length = 0;
            print_newline();
            print_prompt();
        }
        else
        {
            char c = get_char(k);
            if (c != '\0')
            {
                print_char(c);
                strcat(buffer, &c);
                length++;
            }
        }
    }
}

void print_prompt()
{
    print_string("> ");
}

void parse_command(char* cmd)
{
    print_newline();
    if (strncmp(cmd, cmd_read_single, strlen(cmd_read_single)) == 0)
    {
        char* arg = strchr(cmd, ' ') + 1;
        print_newline();
        uint32_t addr = parse_int_hex(arg);
        print_u32_hex(addr);
        print_string(": ");
        uint8_t* addr_ptr = (uint8_t*)addr;
        print_byte_hex(*addr_ptr);
    }
    else
    {
        print_string("Unknown command: ");
        print_string(cmd);
    }
}

