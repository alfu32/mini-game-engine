#ifndef ENTITY_H
#define ENTITY_H
    #include "shape.h"
    #include <stdlib.h>
    #include <stdarg.h>
    #include <stdio.h>

    typedef struct entity_t entity_t;
    // Define a function pointer type for shape transformation
    typedef shape_t (*ShapeTransform)(struct entity_t*, int, char*);

    typedef enum entity_type_t entity_type_t;

    typedef enum entity_type_t{
        BACKGROUND=0x10,
        SHIP=0x11,
        FOE=0x12,
        BULLET=0x13,
        ROCKET=0x14
    } entity_type_t;

    // Define a struct for an animated shape
    typedef struct entity_t {
        entity_type_t type;
        // Flag flags;
        struct shape_t* shape;
        int birth_frame;
        int life;
        int power;
        int collision;
        int team;
        char color;
        char background;
        ShapeTransform next;
        // should always stay last, it will contain exta fields of derived objects
        const char data_map[255];
    } entity_t;

    entity_t* entity__new(int birthFrame,int x0, int y0, const char* content, ShapeTransform transformFunc,const char color,const char background);
    void entity__dealloc(entity_t* entity);
    void entity__print(entity_t* entity);

#endif