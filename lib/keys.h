#ifndef KEYS_H
#define KEYS_H
    #include <unistd.h>
    #include <termios.h> // Include termios header
    #define READKEY_INIT struct termios oldt,newt;\
    tcgetattr(STDIN_FILENO, &oldt);\
    newt = oldt;\
    newt.c_lflag &= ~(ICANON | ECHO);\
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    #define READKEY_DEINIT tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    // Function to check if a key has been pressed (non-blocking)
    int key_is_pressed();
    // Function to read a key if one is pressed (non-blocking)
    int key_read();
    char* keys_get_pressed();
#endif