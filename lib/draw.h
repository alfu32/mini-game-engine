#ifndef DRAW_H
#define DRAW_H
    #include "viewport.h"
    #include "shape.h"
    void terminal_clear();
    void viewport_shape_draw(Viewport* screen, Shape* shape,char fg,char bg);
    char** split_lines(const char* input, int* lineCount);
    char* circular_offset_line(char* line);
    char* circular_offset_string(const char* input);

#endif