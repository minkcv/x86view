#ifndef STIRNG_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

char* strcat(char* dest, char* src);

int strcmp(char* str1, char* str2);

int strncmp(char* str1, char* str2, size_t num);

char* strcpy(char* dest, char* src);

size_t strlen(char* str);

char* strchr(char* str, char c);

uint32_t parse_int(char* str, bool* failure);

uint32_t parse_int_hex(char* str, bool* failure);

#endif

