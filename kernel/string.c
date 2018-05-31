#include "string.h"

char* strcat(char* dest, char* src)
{
    size_t end = strlen(dest) + 1;
    size_t new_length = end + strlen(src);
    int i;
    for (i = end; i < new_length; i++)
        dest[i] = src[i - end];

    return dest;
}

int strcmp(char* str1, char* str2)
{
    int i;
    int length = strlen(str1);
    for (i = 0; i < length; i++)
    {
        if (str1[i] != str2[i])
            return str1[i] - str2[i];
    }
    return 0;
}

char* strcpy(char* dest, char* src)
{
    int i;
    int length = strlen(src);
    for (i = 0; i < length + 1; i++)
        dest[i] = src[i];

    return dest;
}

size_t strlen(char* str)
{
    size_t length = 0;
    while (str[length] != '\0')
        length++;

    return length;
}

