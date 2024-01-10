#ifndef SCENE_H
#define SCENE_H
    #include "entity.h"
    #include "viewport.h"
    // Define a struct for the scene manager
    typedef struct SceneManager {
        entity_t** entities; // Array of pointers to AnimatedShapes
        int entities_count;         // Number of entities in the scene
    } SceneManager;

    // Define a struct that contains object that are colliding
    typedef struct EntityCollision {
        entity_t* a;
        entity_t* b;
    } EntityCollision;

    SceneManager* scene_manager__new();
    void scene_manager__add_entity(SceneManager* manager, entity_t* entity);
    void scene_manager__update(SceneManager* manager, int currentFrame,char pressed[10]);
    void scene_manager__do_collisions(SceneManager* manager);
    void scene_manager__remove_dead_shapes(SceneManager* manager);
    void scene_manager__draw_on_viewport(SceneManager* manager,Viewport* vpp);
    void scene_manager__free(SceneManager* manager);
#endif