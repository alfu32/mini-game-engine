#ifndef KEYS_C
#define KEYS_C
#include "include/keys.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h> // Include termios header

// Function to check if a key has been pressed (non-blocking)
int key_is_pressed()
{
    struct timeval tv;
    fd_set fds;
    fd_set *fds_ref=&fds;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    do
    {
        unsigned int __i;
        fd_set *__arr = fds_ref;
        for (__i = 0; __i < sizeof(fd_set) / sizeof(__fd_mask); ++__i)
            ((__arr)->__fds_bits)[__i] = 0;
    } while (0);
    fds_ref->__fds_bits[0] |= 1L;

    if (select(0 + 1, fds_ref, NULL, NULL, &tv) == -1)
    {
        perror("select");
        exit(1);
    }

    int is_set = ((fds_ref->__fds_bits[0] & 1UL) != 0);
    return is_set;
}

// Function to read a key if one is pressed (non-blocking)
int key_read()
{
    char ch;
    if (read(STDIN_FILENO, &ch, 1) == 1)
    {
        return (int)ch;
    }
    return EOF;
}
char *keys_get_pressed()
{
    char pressed[10] = " ";
    for (int k = 0; k < 10; k++)
    {
        if (key_is_pressed())
        {
            char ch = ' ';
            if (read(STDIN_FILENO, &ch, 1) == 1)
            {
                pressed[k] = ch;
            }
        }
        usleep(500);
    }

    char *str = (char *)malloc(11 * sizeof(char)); // Allocate memory for a 10-character string + null terminator
    if (str == NULL)
    {
        fprintf(stderr, "Memory allocation for pressed keys failed\n");
        exit(1);
    }
    strcpy(str, pressed);
    return str;
}

#endif
