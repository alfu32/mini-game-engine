#include "scene.h"
#include "entity.h"
#include "keys.h"
#include <stdlib.h>
#include <string.h>


// Function to create a new scene manager
SceneManager* scene_manager_new() {
    SceneManager* manager = (SceneManager*)malloc(sizeof(SceneManager));
    manager->entities = NULL;
    manager->entities_count = 0;
    return manager;
}

// Function to add a shape to the scene
void scene_manager_add_shape(SceneManager* manager, Entity* shape) {
    manager->entities_count++;
    manager->entities = realloc(manager->entities, manager->entities_count * sizeof(Entity*));
    manager->entities[manager->entities_count - 1] = shape;
}

// Function to update all entities in the scene
void scene_manager_update(SceneManager* manager, int currentFrame) {
    unsigned int key = 0x0020;
    if (key_is_pressed()) {
        key = key_read();
    }
    for (int i = 0; i < manager->entities_count; i++) {
        Entity* shape = manager->entities[i];
        Shape newShape = shape->next(shape->shape, currentFrame, key );
        shape_copy(shape->shape, newShape);
    }
}
// Function to remove dead shapes (life is 0) from the scene
void scene_manager_remove_dead_shapes(SceneManager* manager) {
    int aliveCount = 0;
    for (int i = 0; i < manager->entities_count; i++) {
        if (manager->entities[i]->life != 0) {
            aliveCount++;
        }
    }

    Entity** aliveShapes = (Entity**)malloc(aliveCount * sizeof(Entity*));
    int index = 0;
    for (int i = 0; i < manager->entities_count; i++) {
        if (manager->entities[i]->life != 0) {
            aliveShapes[index++] = manager->entities[i];
        } else {
            free(manager->entities[i]->shape->content);
            free(manager->entities[i]->shape);
            free(manager->entities[i]);
        }
    }

    free(manager->entities);
    manager->entities = aliveShapes;
    manager->entities_count = aliveCount;
}

// Function to free the scene manager
void scene_manager_free(SceneManager* manager) {
    for (int i = 0; i < manager->entities_count; i++) {
        if (manager->entities[i]->shape->content) {
            free(manager->entities[i]->shape->content);
        }
        free(manager->entities[i]->shape);
        free(manager->entities[i]);
    }
    free(manager->entities);
    free(manager);
}