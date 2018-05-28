#ifndef VGA_CONSOLE_H
#define VGA_CONSOLE_H

#include <stdint.h>

#define CONSOLE_ROWS 25
#define CONSOLE_COLS 80
#define VIDEO_MEMORY_START 0xB8000
#define VGA_WHITE_ON_BLACK 0x0F
#define BYTES_PER_CHAR 2

int cursor_position;

extern void print_byte_hex(uint8_t byte);

extern void print_string(char* str);

extern void print_char(char c);

extern void print_newline();

extern void set_cursor_pos(uint8_t row, uint8_t col);

#endif

