#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

uint8_t pressed_keys;

uint8_t get_keycode();
char get_char(uint8_t keycode);

uint8_t get_key_nointr();
uint8_t get_key_intr();

#endif

