#ifndef SHAPE_H
#define SHAPE_H
    #include "rect.h"
    // Define a struct for a shape
    struct Shape {
        int x;
        int y;
        int z;
        char* content;
    };

    typedef struct Shape Shape;

    Shape* shape_new(int x, int y, const char* content);
    void shape_copy(Shape* this, Shape source);
    void shape_init(Shape* shape, int x, int y, const char* content);
    void shape_move_to(Shape* shape,int x,int y);
    void shape_set(Shape* shape, const char* newContent);
    void shape_set_fmt0(Shape* shape, const char* format, ...);
    void shape__set_content_fmt(Shape* shape, const char* format, ...);
    ClientRect* shape__get_bounding_client_rect(Shape* shape);
    void shape_dealloc(Shape* shape);
#endif