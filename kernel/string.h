#ifndef STIRNG_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

char* strcat(char* dest, char* src);

int strcmp(const char* str1, const char* str2);

int strncmp(const char* str1, const char* str2, size_t num);

char* strcpy(char* dest, char* src);

size_t strlen(const char* str);

char* strchr(char* str, char c);

uint32_t parse_int(char* str, bool* failure);

uint32_t parse_int_hex(char* str, bool* failure);

#endif

