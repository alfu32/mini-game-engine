#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#define BLANK ' '

// Define a struct for the screen buffer
struct Screen {
    int width;
    int height;
    char** buffer;
};
typedef struct Screen Screen;

// Function to allocate memory for the screen buffer
void screen_alloc(Screen* screen) {
    screen->buffer = (char**)malloc(screen->height * sizeof(char*));
    for (int i = 0; i < screen->height; i++) {
        screen->buffer[i] = (char*)malloc(screen->width * sizeof(char));
    }
}

// Function to initialize the screen buffer
void screen_init(Screen* screen) {
    for (int i = 0; i < screen->height; i++) {
        memset(screen->buffer[i], BLANK, screen->width);
    }
}

Screen* screen_new(int width,int height) {
    // Create a screen buffer
    struct Screen* screen=(Screen*)malloc(sizeof(Screen));
    screen->width = 80;  // Adjust to your desired screen width
    screen->height = 24; // Adjust to your desired screen height

    // Allocate memory for the screen buffer
    screen_alloc(screen);

    // Initialize and clear the screen buffer
    screen_init(screen);
    return screen;
}
// Function to clear the screen buffer
void screen_clear(Screen* screen) {
    for (int i = 0; i < screen->height; i++) {
        memset(screen->buffer[i], BLANK, screen->width);
    }
}

// Function to draw a character at a specific position in the screen buffer
void screen_draw_char(Screen* screen, int x, int y, char character) {
    if (x >= 0 && x < screen->width && y >= 0 && y < screen->height) {
        screen->buffer[y][x] = character;
    }
}

// Function to render the screen buffer to the terminal
void screen_renderer(const Screen* screen) {

    putchar('+');
    for (int x = 0; x < screen->width; x++) {
        putchar('-');
    }
    putchar('+');
    putchar('\n');
    for (int y = 0; y < screen->height; y++) {
        putchar('|');
        for (int x = 0; x < screen->width; x++) {
            putchar(screen->buffer[y][x]);
        }
        putchar('|');
        putchar('\n');
    }
    putchar('+');
    for (int x = 0; x < screen->width; x++) {
        putchar('-');
    }
    putchar('+');
    putchar('\n');
}

// Function to deallocate memory for the screen buffer
void screen_dealloc(Screen* screen) {
    for (int i = 0; i < screen->height; i++) {
        free(screen->buffer[i]);
    }
    free(screen->buffer);
    screen->buffer = NULL;
}

