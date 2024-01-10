#ifndef SHAPE_H
#define SHAPE_H
    #include "rect.h"
    // Define a struct for a shape
    struct shape_t {
        int x;
        int y;
        int z;
        char* content;
    };

    typedef struct shape_t shape_t;

    shape_t* shape__new(int x, int y, const char* content);
    void shape__copy(shape_t* this, shape_t source);
    void shape__init(shape_t* shape, int x, int y, const char* content);
    void shape__move_to(shape_t* shape,int x,int y);
    void shape__set(shape_t* shape, const char* newContent);
    void shape__set_fmt0(shape_t* shape, const char* format, ...);
    void shape__set_content_fmt(shape_t* shape, const char* format, ...);
    rectangle_t* shape__get_bounding_client_rect(shape_t* shape);
    void shape__dealloc(shape_t* shape);
#endif