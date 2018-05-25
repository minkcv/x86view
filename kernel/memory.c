#include "memory.h"

void* memcpy(void* destination, void* source, size_t num_bytes)
{
    int i;
    for (i = 0; i < num_bytes; i++)
    {
        *((uint8_t*)(destination + i)) = *((uint8_t*)(source + i));
    }
    return destination;
}

void* memset(void* destination, uint8_t value, size_t num_bytes)
{
    int i;
    for (i = 0; i < num_bytes; i++)
    {
        *((uint8_t*)(destination + i)) = value;
    }
    return destination;
}

