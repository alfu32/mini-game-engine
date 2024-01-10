#ifndef VIEWPORT_C
#define VIEWPORT_C
    #include "viewport.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <termios.h>
    #include <sys/ioctl.h>

    // Function to allocate memory for the viewport buffer
    void viewport__alloc(Viewport* viewport) {
        viewport->buffer = (char**)malloc(viewport->height * sizeof(char*));
        viewport->colors = (char**)malloc(viewport->height * sizeof(char*));
        viewport->backgrounds = (char**)malloc(viewport->height * sizeof(char*));
        for (int i = 0; i < viewport->height; i++) {
            viewport->buffer[i] = (char*)malloc(viewport->width * sizeof(char));
            viewport->colors[i] = (char*)malloc(viewport->width * sizeof(char));
            viewport->backgrounds[i] = (char*)malloc(viewport->width * sizeof(char));
        }
    }

    // Function to initialize the viewport buffer
    void viewport__init(Viewport* viewport) {
        for (int i = 0; i < viewport->height; i++) {
            memset(viewport->buffer[i], BLANK, viewport->width);
            memset(viewport->colors[i], 7, viewport->width);
            memset(viewport->backgrounds[i], 0, viewport->width);
        }
    }

    Viewport* viewport__new(int width,int height) {
        // Create a viewport buffer
        struct Viewport* viewport=(Viewport*)malloc(sizeof(Viewport));
        viewport->width = width;  // Adjust to your desired viewport width
        viewport->height = height; // Adjust to your desired viewport height

        // Allocate memory for the viewport buffer
        viewport__alloc(viewport);

        // Initialize and clear the viewport buffer
        viewport__init(viewport);
        return viewport;
    }
    // Function to clear the viewport buffer
    void viewport__clear(Viewport* viewport) {
        for (int i = 0; i < viewport->height; i++) {
            memset(viewport->buffer[i], BLANK, viewport->width);
            memset(viewport->colors[i], 7, viewport->width);
            memset(viewport->backgrounds[i], 0, viewport->width);
        }
    }

    // Function to draw a character at a specific position in the viewport buffer
    void viewport__draw_char(Viewport* viewport, int x, int y, char character,char fg,char bg) {
        if (x >= 0 && x < viewport->width && y >= 0 && y < viewport->height) {
            viewport->buffer[y][x] = character;
            viewport->colors[y][x] = fg;
            viewport->backgrounds[y][x] = fg;
        }
    }
    const char* colors[]={
        COLOR_RESET,          /// 0
        COLOR_RED,            /// 1
        COLOR_GREEN,          /// 2
        COLOR_YELLOW,         /// 3
        COLOR_BLUE,           /// 4
        COLOR_MAGENTA,        /// 5
        COLOR_CYAN,           /// 6
        COLOR_WHITE,          /// 7
        COLOR_BRIGHT_BLACK,   /// 8
        COLOR_BRIGHT_RED,     /// 9
        COLOR_BRIGHT_GREEN,   /// 10
        COLOR_BRIGHT_YELLOW,  /// 11
        COLOR_BRIGHT_BLUE,    /// 12
        COLOR_BRIGHT_MAGENTA, /// 13
        COLOR_BRIGHT_CYAN,    /// 14
        COLOR_BRIGHT_WHITE    /// 15
    };
    const char* backgrounds[]={
        BACKGROUND_COLOR_BLACK,          /// 0 
        BACKGROUND_COLOR_RED,            /// 1 
        BACKGROUND_COLOR_GREEN,          /// 2 
        BACKGROUND_COLOR_YELLOW,         /// 3 
        BACKGROUND_COLOR_BLUE,           /// 4 
        BACKGROUND_COLOR_MAGENTA,        /// 5 
        BACKGROUND_COLOR_CYAN,           /// 6 
        BACKGROUND_COLOR_WHITE,          /// 7 
        BACKGROUND_BRIGHT_COLOR_BLACK,   /// 8 
        BACKGROUND_BRIGHT_COLOR_RED,     /// 9 
        BACKGROUND_BRIGHT_COLOR_GREEN,   /// 10 
        BACKGROUND_BRIGHT_COLOR_YELLOW,  /// 11 
        BACKGROUND_BRIGHT_COLOR_BLUE,    /// 12 
        BACKGROUND_BRIGHT_COLOR_MAGENTA, /// 13 
        BACKGROUND_BRIGHT_COLOR_CYAN,    /// 14 
        BACKGROUND_BRIGHT_COLOR_WHITE    /// 15 
    };
    // Function to render the viewport buffer to the terminal
    void viewport__renderer(const Viewport* viewport) {
        putchar('+');
        for (int x = 0; x < viewport->width; x++) {
            putchar('-');
        }
        putchar('+');
        putchar('\n');
        for (int y = 0; y < viewport->height; y++) {
            putchar('|');
            int last_color=0;
            for (int x = 0; x < viewport->width; x++) {
                char colored=0;
                //// if(viewport->backgrounds[y][x]){
                ////     printf("%s",backgrounds[viewport->backgrounds[y][x]]);
                ////     colored=1;
                //// }
                if(viewport->colors[y][x]){
                    printf("%s",colors[viewport->colors[y][x]]);
                }
                putchar(viewport->buffer[y][x]);
                if(viewport->colors[y][x] && viewport->colors[y][x] != viewport->colors[y][x+1]){
                    printf("%s",COLOR_RESET);
                }
            }
            printf("%s",COLOR_RESET);
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
    void viewport__dealloc(Viewport* viewport) {
        for (int i = 0; i < viewport->height; i++) {
            free(viewport->buffer[i]);
        }
        free(viewport->buffer);
        viewport->buffer = NULL;
    }
#endif
