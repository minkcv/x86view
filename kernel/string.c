#include "string.h"

char* strcat(char* dest, char* src)
{
    size_t end = strlen(dest);
    size_t new_length = end + strlen(src);
    int i;
    for (i = end; i < new_length; i++)
        dest[i] = src[i - end];

    return dest;
}

int strcmp(char* str1, char* str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t lower_length = len1 < len2 ? len1 : len2;
    if (len1 != len2)
        return lower_length;

    return strncmp(str1, str2, lower_length);
}

int strncmp(char* str1, char* str2, size_t num)
{
    int i;
    for (i = 0; i < num; i++)
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

char* strchr(char* str, char c)
{
    int i;
    for (i = 0; str[i] != c; i++)
        if (str[i] == '\0')
            return NULL;

    return &(str[i]);
}

uint32_t parse_int(char* str, bool* failure)
{
    uint32_t value = 0;
    size_t length = strlen(str);
    int i;
    for (i = 0; i < length; i++)
    {
        int place = (length - i - 1);
        int weight = 1;
        int pow;
        for (pow = 0; pow < place; pow++)
            weight *= 10;
        if (str[i] < 48 || str[i] > 57)
        {
            *failure = true;
            return 0; // Couldn't parse.
        }
        value += (str[i] - 48) * weight;
    }
    return value;
}

uint32_t parse_int_hex(char* str, bool* failure)
{
    uint32_t value = 0;
    int i;
    size_t length = strlen(str);
    for (i = 0; i < length; i++)
    {
        int place = (length - i - 1);
        int weight = 1;
        int pow;
        for (pow = 0; pow < place; pow++)
            weight *= 16;
        if (str[i] > 47 && str[i] < 58)
            value += (str[i] - 48) * weight;
        else if (str[i] > 64 && str[i] < 71)
            value += (str[i] - 65 + 10) * weight;
        else
        {
            *failure = true;
            return 0;
        }
    }
    return value;
}

