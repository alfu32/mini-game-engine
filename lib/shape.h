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

    Shape* shape__new(int x, int y, const char* content);
    void shape__copy(Shape* this, Shape source);
    void shape__init(Shape* shape, int x, int y, const char* content);
    void shape__move_to(Shape* shape,int x,int y);
    void shape__set(Shape* shape, const char* newContent);
    void shape__set_fmt0(Shape* shape, const char* format, ...);
    void shape__set_content_fmt(Shape* shape, const char* format, ...);
    ClientRect* shape__get_bounding_client_rect(Shape* shape);
    void shape__dealloc(Shape* shape);
#endif