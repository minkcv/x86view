#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void* memcpy(void* destination, void* source, size_t num_bytes);
void* memset(void* destination, uint8_t value, size_t num_bytes);

#endif

