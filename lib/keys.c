#include "keys.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h> // Include termios header

// Function to check if a key has been pressed (non-blocking)
int key_is_pressed() {
    struct timeval tv;
    fd_set fds;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) == -1) {
        perror("select");
        exit(1);
    }

    return FD_ISSET(STDIN_FILENO, &fds);
}

// Function to read a key if one is pressed (non-blocking)
int key_read() {
    char ch;
    if (read(STDIN_FILENO, &ch, 1) == 1) {
        return (int)ch;
    }
    return EOF;
}

