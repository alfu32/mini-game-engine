#ifndef ENTITY_H
#define ENTITY_H
    #include "shape.h"
    #include <stdlib.h>
    #include <stdarg.h>
    #include <stdio.h>

    typedef struct Entity Entity;
    // Define a function pointer type for shape transformation
    typedef Shape (*ShapeTransform)(struct Entity*, int, char*);

    typedef enum EntityType EntityType;

    typedef enum EntityType{
        BACKGROUND=0x10,
        SHIP=0x11,
        FOE=0x12,
        BULLET=0x13,
        ROCKET=0x14
    } EntityType;

    // Define a struct for an animated shape
    typedef struct Entity {
        EntityType type;
        // Flag flags;
        struct Shape* shape;
        int birth_frame;
        int life;
        int power;
        int collision;
        char color;
        char background;
        ShapeTransform next;
    } Entity;

    Entity* entity_new(int birthFrame,int x0, int y0, const char* content, ShapeTransform transformFunc,const char color,const char background);
    void entity_dealloc(Entity* entity);
    void entity__print(Entity* entity);

#endif