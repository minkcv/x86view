#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

char get_key();

uint8_t get_key_nointr();
uint8_t get_key_intr();

#endif

