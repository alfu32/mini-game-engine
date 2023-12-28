#include "entity.h"
#include "shape.h"
#include <stdlib.h>
#include <stdarg.h>

// Function to create an animated shape
Entity* entity_new(int birthFrame,int x0, int y0, const char* content, ShapeTransform transformFunc) {
    Entity* entity=(Entity*)malloc(sizeof(Entity));
    entity->shape = shape_new(x0,y0,content);
    entity->birth_frame = birthFrame;
    entity->life = 1;
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

