#ifndef RECT_H
#define RECT_H
    #include "shape.h"
    #include <string.h>

    // Struct for a shape
    // Struct for a client rect
    struct ClientRect {
        int x;
        int y;
        unsigned int width;
        unsigned int height;
    };
    typedef struct ClientRect ClientRect;
    ClientRect get_bounding_client_rect(Shape* shape);
#endif