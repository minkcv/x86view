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
    console_clear();
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
    bool parse_failure;
    char* args = strchr(cmd, ' ');
    if (args == NULL)
    {
        print_usage(cmd);
        return;
    }
    *(args) = '\0';
    args++; // Move pointer past '\0' character.
    if (strcmp(cmd, cmd_read) == 0)
    {
        uint32_t n = 1; // Number of bytes to print.
        char* arg1 = args;
        char* arg2 = strchr(arg1, ' ');
        if (arg2 != NULL)
        {
            *(arg2) = '\0';
            arg2++; // Move pointer past '\0' character.
            parse_failure = false;
            n = parse_int_hex(arg2, &parse_failure);
            if (parse_failure || n == 0)
            {
                // Either failed to parse int or user entered 0.
                // Neither of which are allowed.
                print_usage(cmd);
                return;
            }
        }

        parse_failure = false;
        uint32_t addr = parse_int_hex(arg1, &parse_failure);
        if (parse_failure)
        {
            print_usage(cmd);
            return;
        }
        int i;
        for (i = 0; i < n; i++)
        {
            if (i % 16 == 0)
            {
                print_newline();
                print_u32_hex(addr + i);
                print_string(": ");
            }
            if (i != 0 && i % 2 == 0 && i % 16 != 0)
                print_char(' ');

            uint8_t* addr_ptr = (uint8_t*)(addr + i);
            print_byte_hex(*addr_ptr);
        }
    }
    else
    {
        print_string("Unknown command: ");
        print_string(cmd);
    }
}

void print_usage(char* cmd)
{
    if (strcmp(cmd, cmd_read) == 0)
    {
        print_string("R - Read bytes - Usage:\n");
        print_string("\tR address [number of bytes]\n");
        print_string("\t- address: Memory address in hexadecimal to start reading at.\n");
        print_string("\t- [number of bytes]: A nonzero number of bytes to read in hexadecimal.\n");
        print_string("\t\tImplicitly 1 if not specified.\n");
    }
    else if (strcmp(cmd, cmd_write) == 0)
    {
        print_string("W - Write bytes - Usage:\n");
        print_string("W address values\n");
        print_string("\taddress: Memory address in hexadecimal to write bytes to.\n");
        print_string("\tvalues: Bytes to write to the memory address specified.\n");
        print_string("\t\tMust be an even number of hex digits.\n");
    }
}

