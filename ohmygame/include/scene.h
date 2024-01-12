#ifndef SCENE_H
#define SCENE_H
    #include "entity.h"
    #include "viewport.h"
    // Define a struct for the scene manager
    typedef struct scene_manager_t {
        entity_t** entities; // Array of pointers to AnimatedShapes
        int entities_count;         // Number of entities in the scene
    } scene_manager_t;

    // Define a struct that contains object that are colliding
    typedef struct entity_collision_t {
        entity_t* a;
        entity_t* b;
    } entity_collision_t;

    scene_manager_t* scene_manager__new();
    void scene_manager__add_entity(scene_manager_t* manager, entity_t* entity);
    void scene_manager__update(scene_manager_t* manager, int currentFrame,char *pressed);
    void scene_manager__do_collisions(scene_manager_t* manager);
    void scene_manager__remove_dead_shapes(scene_manager_t* manager);
    void scene_manager__draw_on_viewport(scene_manager_t* manager,viewport_t* vpp);
    void scene_manager__free(scene_manager_t* manager);
#endif