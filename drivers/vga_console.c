#include "drivers/vga_console.h"
#include "kernel/memory.h"

void print_byte_hex(uint8_t byte)
{
    uint8_t hex_digit_1 = (byte >> 4) & 0xF;
    uint8_t hex_digit_2 = byte & 0xF;
    print_hex_digit(hex_digit_1);
    print_hex_digit(hex_digit_2);
}

void print_hex_digit(uint8_t value)
{
    uint8_t* memptr = (uint8_t*) VIDEO_MEMORY_START + cursor_position;
    if (value < 10)
        memset(memptr, value + 48, 1);
    else
        memset(memptr, value + 55, 1);
    memset(memptr + 1, VGA_WHITE_ON_BLACK, 1);
    cursor_position += BYTES_PER_CHAR;
}

void print_u32_hex(uint32_t i)
{
    print_hex_digit((i >> 28) & 0xF);
    print_hex_digit((i >> 24) & 0xF);
    print_hex_digit((i >> 20) & 0xF);
    print_hex_digit((i >> 16) & 0xF);
    print_hex_digit((i >> 12) & 0xF);
    print_hex_digit((i >> 8) & 0xF);
    print_hex_digit((i >> 4) & 0xF);
    print_hex_digit(i & 0xF);
}

void print_string(char* str)
{
    int i;
    for (i = 0; *(str + i) != '\0'; i++)
    {
        if (*(str + i) == '\n')
            print_newline();
        else if (*(str + i) == '\t')
            print_string("    "); // 4 spaces
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
    uint8_t current_row = (cursor_position / BYTES_PER_CHAR) / CONSOLE_COLS;
    if (current_row >= CONSOLE_ROWS - 1)
    {
        console_scroll_down();
        cursor_position = current_row * CONSOLE_COLS * BYTES_PER_CHAR;
    }
    else
        cursor_position = ((current_row + 1) * CONSOLE_COLS) * BYTES_PER_CHAR;
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

void console_scroll_down()
{
    uint8_t* memptr = (uint8_t*) VIDEO_MEMORY_START;
    uint8_t* memptr_second_line = memptr + BYTES_PER_CHAR * CONSOLE_COLS;
    size_t all_lines_but_one = 
        (BYTES_PER_CHAR * CONSOLE_COLS * CONSOLE_COLS) - 
        (BYTES_PER_CHAR * CONSOLE_COLS);
    memcpy(memptr, memptr_second_line, all_lines_but_one);
}

void set_cursor_pos(uint8_t row, uint8_t col)
{
    cursor_position = (row * CONSOLE_COLS + col) * BYTES_PER_CHAR;
}

