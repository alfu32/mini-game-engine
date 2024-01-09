#ifndef ENTITY_C
#define ENTITY_C
    #include "entity.h"
    #include "shape.h"
    #include <stdlib.h>
    #include <stdarg.h>

    // Function to create an animated shape
    Entity* entity_new(int birthFrame,int x0, int y0, const char* content, ShapeTransform transformFunc,const char color,const char background) {
        Entity* entity=(Entity*)malloc(sizeof(Entity));
        entity->type = SHIP;
        entity->shape = shape_new(x0,y0,content);
        entity->birth_frame = birthFrame;
        entity->life = 1;
        entity->power=1;
        entity->collision=1;
        entity->color=color;
        entity->background=background;
        entity->next = transformFunc;
        return entity;
    }

    // Function to destroy an animated shape
    void entity_dealloc(Entity* entity) {
        if (entity->shape != NULL) {
            shape_dealloc(entity->shape);
            free(entity->shape);
        }
        entity->shape = NULL;
        free(entity);
    }
    void entity__print(Entity* entity) {
        printf("Entity {\n\
            type        : %d\n\
            shape       : %s\n\
            birth_frame : %d\n\
            life        : %d\n\
            power       : %d\n\
            collision   : %d\n\
            color       : %d\n\
            background  : %d\n\
        }\n",
            entity->type,
            entity->shape,
            entity->birth_frame,
            entity->life,
            entity->power,
            entity->collision,
            entity->color,
            entity->background
        );
    }
#endif
