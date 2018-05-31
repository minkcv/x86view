#include "drivers/vga_console.h"
#include "kernel/memory.h"

void print_byte_hex(uint8_t byte)
{
    uint8_t* memptr = (uint8_t*) VIDEO_MEMORY_START + cursor_position;
    if (byte < 10)
        memset(memptr, byte + 48, 1);
    else
        memset(memptr, byte + 55, 1);
    memset(memptr + 1, VGA_WHITE_ON_BLACK, 1);
    cursor_position += BYTES_PER_CHAR;
}

void print_string(char* str)
{
    int i;
    for (i = 0; *(str + i) != '\0'; i++)
    {
        if (*(str + i) == '\n')
            print_newline();
        else
            print_char(*(str + i));
    }
}

void print_char(char c)
{
    if (c == '\n')
    {
        print_newline();
        return;
    }
    uint8_t* memptr = (uint8_t*) VIDEO_MEMORY_START + cursor_position;
    memset(memptr, c, 1);
    memset(memptr + 1, VGA_WHITE_ON_BLACK, 1);
    cursor_position += BYTES_PER_CHAR;
}

void print_newline()
{
    uint8_t rows = (cursor_position / BYTES_PER_CHAR) / CONSOLE_COLS;
    cursor_position = ((rows + 1) * CONSOLE_COLS) * BYTES_PER_CHAR;
}

void console_backspace()
{
    if (cursor_position == 0)
        return;

    cursor_position -= BYTES_PER_CHAR;
    print_char(' ');
    cursor_position -= BYTES_PER_CHAR;
}

void console_clear()
{
    set_cursor_pos(0, 0);
    int i;
    for (i = 0; i < CONSOLE_COLS * CONSOLE_ROWS; i++)
    {
        print_char(' ');
    }
    set_cursor_pos(0, 0);
}

void set_cursor_pos(uint8_t row, uint8_t col)
{
    cursor_position = (row * CONSOLE_COLS + col) * BYTES_PER_CHAR;
}

