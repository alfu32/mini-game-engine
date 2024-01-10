#ifndef RECT_H
#define RECT_H
    #include <string.h>

    // Struct for a shape
    // Struct for a client rect
    struct rectangle_t {
        int x;
        int y;
        int z;
        unsigned int width;
        unsigned int height;
    };
    typedef struct rectangle_t rectangle_t;
    int rectangle__intersects(const struct rectangle_t* rect1, const struct rectangle_t* rect2);
    int rectangle__static__between(int x, int a, int b);
    int rectangle__contains_point(const struct rectangle_t* rect1, int x3, int y3);
    int rectangle__free(struct rectangle_t* rect1);
#endif