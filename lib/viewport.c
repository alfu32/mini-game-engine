#include "viewport.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

// Function to allocate memory for the viewport buffer
void viewport_alloc(Viewport* viewport) {
    viewport->buffer = (char**)malloc(viewport->height * sizeof(char*));
    for (int i = 0; i < viewport->height; i++) {
        viewport->buffer[i] = (char*)malloc(viewport->width * sizeof(char));
    }
}

// Function to initialize the viewport buffer
void viewport_init(Viewport* viewport) {
    for (int i = 0; i < viewport->height; i++) {
        memset(viewport->buffer[i], BLANK, viewport->width);
    }
}

Viewport* viewport_new(int width,int height) {
    // Create a viewport buffer
    struct Viewport* viewport=(Viewport*)malloc(sizeof(Viewport));
    viewport->width = 80;  // Adjust to your desired viewport width
    viewport->height = 24; // Adjust to your desired viewport height

    // Allocate memory for the viewport buffer
    viewport_alloc(viewport);

    // Initialize and clear the viewport buffer
    viewport_init(viewport);
    return viewport;
}
// Function to clear the viewport buffer
void viewport_clear(Viewport* viewport) {
    for (int i = 0; i < viewport->height; i++) {
        memset(viewport->buffer[i], BLANK, viewport->width);
    }
}

// Function to draw a character at a specific position in the viewport buffer
void viewport_draw_char(Viewport* viewport, int x, int y, char character) {
    if (x >= 0 && x < viewport->width && y >= 0 && y < viewport->height) {
        viewport->buffer[y][x] = character;
    }
}

// Function to render the viewport buffer to the terminal
void viewport_renderer(const Viewport* viewport) {

    putchar('+');
    for (int x = 0; x < viewport->width; x++) {
        putchar('-');
    }
    putchar('+');
    putchar('\n');
    for (int y = 0; y < viewport->height; y++) {
        putchar('|');
        for (int x = 0; x < viewport->width; x++) {
            putchar(viewport->buffer[y][x]);
        }
        putchar('|');
        putchar('\n');
    }
    putchar('+');
    for (int x = 0; x < viewport->width; x++) {
        putchar('-');
    }
    putchar('+');
    putchar('\n');
}

// Function to deallocate memory for the viewport buffer
void viewport_dealloc(Viewport* viewport) {
    for (int i = 0; i < viewport->height; i++) {
        free(viewport->buffer[i]);
    }
    free(viewport->buffer);
    viewport->buffer = NULL;
}

