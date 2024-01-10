#ifndef SCENE_C
#define SCENE_C
    #include "scene.h"
    #include "entity.h"
    #include "viewport.h"
    #include "draw.h"
    #include "keys.h"
    #include "rect.h"
    #include <stdlib.h>
    #include <string.h>


    // Function to create a new scene manager
    SceneManager* scene_manager__new() {
        SceneManager* manager = (SceneManager*)malloc(sizeof(SceneManager));
        manager->entities = NULL;
        manager->entities_count = 0;
        return manager;
    }

    // Function to add a entity to the scene
    void scene_manager__add_entity(SceneManager* manager, entity_t* entity) {
        manager->entities_count++;
        manager->entities = realloc(manager->entities, manager->entities_count * sizeof(entity_t*));
        manager->entities[manager->entities_count - 1] = entity;
    }

    // Function to update all entities in the scene
    void scene_manager__update(SceneManager* manager, int currentFrame,char *keys) {
        /// unsigned int key = 0x0020;
        /// if (key_is_pressed()) {
        ///     key = key_read();
        /// }
        for (int i = 0; i < manager->entities_count; i++) {
            entity_t* entity = manager->entities[i];
            /// Shape newShape = entity->next(entity, currentFrame, key );
            Shape newShape = entity->next(entity, currentFrame, keys );
            shape__copy(entity->shape, newShape);
        }
    }
    // Function to remove dead shapes (life is 0) from the scene
    void scene_manager__do_collisions(SceneManager* manager) {
        EntityCollision* collisions = (EntityCollision*)malloc(sizeof(EntityCollision)*(manager->entities_count)*(manager->entities_count));
        unsigned int collision_count=0;
        for (int i = 0; i < manager->entities_count; i++) {
            if(i>0){
                for(int j=0;j<i;j++){
                    entity_t* ea = manager->entities[i];
                    entity_t* eb = manager->entities[j];
                    if(ea->team != eb->team && eb->collision != 0 && ea->collision != 0 && ea->shape->z == eb->shape->z ){
                        ClientRect* a = shape__get_bounding_client_rect(ea->shape);
                        ClientRect* b = shape__get_bounding_client_rect(eb->shape);
                        if( client_rect__intersects(a,b)){
                            EntityCollision cl;
                            cl.a=ea;
                            cl.b=eb;
                            collisions[collision_count++]=cl;
                        }
                        free(a);
                        free(b);
                    }
                }
            }
        }
        for(int k=0;k<collision_count;k++){
            entity_t* a=collisions[k].a;
            entity_t* b=collisions[k].b;
            int la = a->power;
            int lb = b->power;
            a->life-=lb;
            b->life-=la;
            if ( a->life < 0 ) {
                a->life = 0;
            }
            if ( b->life < 0 ) {
                b->life = 0;
            }
        }
        

        free(collisions);
    }
    // Function to remove dead shapes (life is 0) from the scene
    void scene_manager__remove_dead_shapes(SceneManager* manager) {
        int aliveCount = 0;
        for (int i = 0; i < manager->entities_count; i++) {
            if (manager->entities[i]->life != 0) {
                aliveCount++;
            }
        }

        entity_t** aliveShapes = (entity_t**)malloc(aliveCount * sizeof(entity_t*));
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
    void scene_manager__draw_on_viewport(SceneManager* manager, Viewport* vp) {
        if (manager == NULL || vp == NULL) {
            return;
        }

        for (int i = 0; i < manager->entities_count; i++) {
            entity_t* entity = manager->entities[i];
            if (entity != NULL && entity->life > 0 && entity->shape != NULL) {
                viewport_shape_draw(vp, entity->shape,entity->color,entity->background);
            }
        }
    }
    // Function to free the scene manager
    void scene_manager__free(SceneManager* manager) {
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
#endif