#include "prompt.h"
#include "string.h"
#include "memory.h"
#include "io.h"
#include "drivers/scancodes.h"
#include "drivers/keyboard.h"
#include "drivers/vga_console.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


void prompt_run()
{
    uint8_t max_cmd_length = CONSOLE_COLS - 2;
    char buffer[max_cmd_length];
    memset(&buffer, 0, max_cmd_length);
    size_t length = 0;
    console_clear();
    print_string("\t\t\tx86view by Will Smith - github.com/minkcv/x86view\n\n");
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
        else if (length < max_cmd_length)
        {
            char c = get_char(k);
            if (c != '\0')
            {
                print_char(c);
                buffer[length] = c;
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
    if (strcmp(cmd, cmd_help) == 0)
    {
        print_usage(cmd_read);
        print_usage(cmd_write);
        print_usage(cmd_jump);
        print_usage(cmd_find);
        return;
    }
    bool parse_failure; // Used later when parsing ints.
    char* args = strchr(cmd, ' ');
    if (args != NULL)
    {
        *(args) = '\0';
        args++; // Move pointer past '\0' character.
    }
    if (strcmp(cmd, cmd_read) == 0)
    {
        if (args == NULL)
        {
            print_usage(cmd);
            return;
        }
        uint32_t n = 1; // Number of bytes to print.
        char* arg1 = args;
        char* arg2 = strchr(arg1, ' ');
        if (arg2 != NULL)
        {
            *(arg2) = '\0';
            arg2++; // Move pointer past '\0' character.
            n = parse_int_hex(arg2, &parse_failure);
            if (parse_failure || n == 0)
            {
                // Either failed to parse int or user entered 0.
                // Neither of which are allowed.
                print_usage(cmd);
                return;
            }
        }

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
    else if (strcmp(cmd, cmd_write) == 0)
    {
        if (args == NULL)
        {
            print_usage(cmd);
            return;
        }
        char* arg1 = args;
        char* arg2 = strchr(arg1, ' ');
        size_t n_digits;
        if (arg2 != NULL)
        {
            *(arg2) = '\0';
            arg2++; // Move pointer past '\0' character.
            n_digits = strlen(arg2);
            parse_int_hex(arg2, &parse_failure);
            if (parse_failure || n_digits % 2 != 0)
            {
                // Either failed to parse int or user entered 
                // a non even number of digits (need pairs to make bytes).
                print_usage(cmd);
                return;
            }
        }
        else
        {
            // Second argument is required.
            print_usage(cmd);
            return;
        }
        uint32_t addr = parse_int_hex(arg1, &parse_failure);
        if (parse_failure)
        {
            print_usage(cmd);
            return;
        }
        // Convert pairs of hex digits to bytes and write to memory.
        char byte_str[3];
        byte_str[2] = '\0';
        int i;
        for (i = 0; i < n_digits; i += 2)
        {
            byte_str[0] = arg2[i];
            byte_str[1] = arg2[i + 1];
            uint8_t byte = parse_int_hex(byte_str, &parse_failure);
            if (parse_failure)
            {
                print_usage(cmd);
                return;
            }
            uint8_t* addr_ptr = (uint8_t*)addr + i / 2;
            *addr_ptr = byte;
        }
    }
    else if (strcmp(cmd, cmd_jump) == 0)
    {
        if (args == NULL)
        {
            print_usage(cmd);
            return;
        }
        uint32_t addr = parse_int_hex(args, &parse_failure);
        if (parse_failure)
        {
            print_usage(cmd);
            return;
        }
        asm volatile ("jmp %0" : : "a"(addr));
    }
    else if (strcmp(cmd, cmd_find) == 0)
    {
        if (args == NULL)
        {
            print_usage(cmd);
            return;
        }
        char* arg1 = args;
        char* arg2 = strchr(arg1, ' ');
        if (arg2 != NULL)
        {
            // Advance past the space.
            *(arg2) = '\0';
            arg2++;
        }
        else
        {
            print_usage(cmd);
            return;
        }
        char* arg3 = strchr(arg2, ' ');
        size_t n_digits;
        if (arg3 != NULL)
        {
            *(arg3) = '\0';
            arg3++; // Move past '\0'
            n_digits = strlen(arg3);
            parse_int_hex(arg3, &parse_failure);
            if (parse_failure || n_digits % 2 != 0)
            {
                // Failure to parse
                print_usage(cmd);
                return;
            }
        }
        else
        {
            print_usage(cmd);
            return;
        }
        uint32_t start_address = parse_int_hex(arg1, &parse_failure);
        uint32_t end_address = parse_int_hex(arg2, &parse_failure);
        if (parse_failure)
        {
            print_usage(cmd);
            return;
        }
        uint32_t i;
        char byte_str[3];
        byte_str[2] = '\0';
        bool first = true;
        for (i = start_address; i < end_address; i++)
        {
            bool match = true;
            uint8_t n;
            for (n = 0; n < n_digits; n += 2)
            {
                byte_str[0] = arg3[n];
                byte_str[1] = arg3[n + 1];
                uint8_t byte = parse_int_hex(byte_str, &parse_failure);
                if (parse_failure)
                {
                    print_usage(cmd);
                    return;
                }
                if ((*(uint8_t*)(i + n / 2)) != byte)
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                if (!first)
                    print_string(", ");
                print_u32_hex(i);
                first = false;
            }
        }
    }
    else
    {
        print_string("Unknown command. Try HELP");
    }
}

void print_usage(const char* cmd)
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
        print_string("\tW address values\n");
        print_string("\t- address: Memory address in hexadecimal to write bytes to.\n");
        print_string("\t- values: Bytes to write to the memory address specified.\n");
        print_string("\t\tMust be an even number of hex digits.\n");
    }
    else if (strcmp(cmd, cmd_jump) == 0)
    {
        print_string("J - Jump to address - Usage:\n");
        print_string("\tJ address\n");
        print_string("\t- address: Absolute memory address in hex to jump to and start executing.\n");
    }
    else if (strcmp(cmd, cmd_find) == 0)
    {
        print_string("FIND - Find a sequence of bytes in memory - Usage:\n");
        print_string("\tFIND start end sequence\n");
        print_string("\t- start: Memory address to start searching at.\n");
        print_string("\t- end: Memory address to stop searching at.\n");
        print_string("\t- sequence: Sequence of bytes to search for between the start and end.\n");
    }
}

