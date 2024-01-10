#ifndef RECT_H
#define RECT_H
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
    int client_rect__intersects(const struct ClientRect* rect1, const struct ClientRect* rect2);
    int client_rect__static__between(int x, int a, int b);
    int client_rect__contains_point(const struct ClientRect* rect1, int x3, int y3);
    int client_rect__free(struct ClientRect* rect1);
#endif