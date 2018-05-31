#include "prompt.h"
#include "memory.h"
#include "drivers/scancodes.h"
#include "drivers/keyboard.h"
#include "drivers/vga_console.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


void prompt_run()
{
    cmd_read_single = "r";
    char* buffer = (char*)0x7E00; // TODO where to put this in memory?
    size_t length = 0;
    print_prompt();
    while (true)
    {
        uint8_t k = get_keycode();
        if (k == KEY_BACKSPACE && length > 0)
        {
            buffer[length] = '\0';
            length--;
            console_backspace();
        }
        else if (k == KEY_ENTER)
        {
            parse_command(buffer);
            memset(buffer, 0, length);
            print_newline();
            print_prompt();
        }
        else
        {
            char c = get_char(k);
            if (c != '\0')
            {
                print_char(c);
                length++;
            }
            //strcat(buffer, (char*)c);
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
    print_string(cmd);
}

