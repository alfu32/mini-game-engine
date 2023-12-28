#ifndef DRAW_H
#define DRAW_H
    #include "viewport.h"
    #include "shape.h"
    void terminal_clear();
    void viewport_shape_draw(Viewport* screen, Shape* shape);

#endif