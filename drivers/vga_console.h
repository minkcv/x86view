#ifndef VGA_CONSOLE_H
#define VGA_CONSOLE_H

#include <stdint.h>

#define CONSOLE_ROWS 25
#define CONSOLE_COLS 80
#define VIDEO_MEMORY_START 0xB8000
#define VGA_WHITE_ON_BLACK 0x0F
#define BYTES_PER_CHAR 2

int cursor_position;

void print_byte_hex(uint8_t byte);

void print_hex_digit(uint8_t value);

void print_u32_hex(uint32_t i);

void print_string(char* str);

void print_char(char c);

void print_newline();

void console_backspace();

void console_clear();

void console_scroll_down();

void set_cursor_pos(uint8_t row, uint8_t col);

#endif

