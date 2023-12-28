#ifndef ENTITY_H
#define ENTITY_H
    #include "shape.h"
    #include <stdlib.h>
    #include <stdarg.h>
    
    typedef struct Entity Entity;
    // Define a function pointer type for shape transformation
    typedef Shape (*ShapeTransform)(struct Entity*, int, char);

    // Define a struct for an animated shape
    typedef struct Entity {
        struct Shape* shape;
        int birth_frame;
        int life;
        ShapeTransform next;
    } Entity;

    Entity* entity_new(int birthFrame,int x0, int y0, const char* content, ShapeTransform transformFunc);
    void entity_dealloc(Entity* entity);

#endif