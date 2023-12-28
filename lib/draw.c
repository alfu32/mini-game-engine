#include "draw.h"
#include "viewport.h"
#include "shape.h"
#include <stdio.h>

// Function to clear the entire terminal screen
void terminal_clear() {
    printf("\e[1;1H\e[2J");
}
// Function to draw a shape onto the screen buffer
void viewport_shape_draw(Viewport* screen, Shape* shape) {
    int y = shape->y;
    int x = shape->x+1;
    char* content = shape->content;

    while (*content) {
        if(*content != BLANK){
            viewport_draw_char(screen, x, y, *content);
        }
        content++;
        x++;
    }
}