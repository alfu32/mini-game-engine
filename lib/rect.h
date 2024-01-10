#ifndef RECT_H
#define RECT_H
    #include "shape.h"
    #include <string.h>

    // Struct for a shape
    // Struct for a client rect
    struct ClientRect {
        int x;
        int y;
        int z;
        unsigned int width;
        unsigned int height;
    };
    typedef struct ClientRect ClientRect;
    ClientRect* get_bounding_client_rect(Shape* shape);
    int client_rect__intersects(const struct ClientRect* rect1, const struct ClientRect* rect2);
    int client_rect__static__between(int x, int a, int b);
    int client_rect__contains_point(const struct ClientRect* rect1, int x3, int y3);
#endif