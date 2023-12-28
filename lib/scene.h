#ifndef SCENE_H
#define SCENE_H
    #include "entity.h"
    #include "viewport.h"
    // Define a struct for the scene manager
    typedef struct SceneManager {
        Entity** entities; // Array of pointers to AnimatedShapes
        int entities_count;         // Number of entities in the scene
    } SceneManager;

    SceneManager* scene_manager_new();
    void scene_manager_add_entity(SceneManager* manager, Entity* entity);
    void scene_manager_update(SceneManager* manager, int currentFrame);
    void scene_manager_remove_dead_shapes(SceneManager* manager);
    void scene_manager_draw_on_viewport(SceneManager* manager,Viewport* vpp);
    void scene_manager_free(SceneManager* manager);
#endif