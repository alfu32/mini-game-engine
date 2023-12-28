#include "lib/lib.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>



// Create a vpp buffer
Viewport* vpp;

// Create a scene manager
SceneManager* manager;
// declare behaviours
Shape player_behaviour_next(Entity* e, int frame, char key);
Shape foe_behaviour_next(Entity* e, int frame, char key);
Shape bullet_behaviour(Entity* e, int frame, char key);

int foe_direction=1;


// Define two different 'next' behaviors for the shapes
Shape player_behaviour_next(Entity* e, int frame, char key) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,""};
    sh.content=strdup(e->shape->content);
    switch(key){
        case 'z':
            sh.x-=1;
            break;
        case 'c':
            sh.x+=1;
            break;
        case 's':
            sh.y-=1;
            break;
        case 'x':
            sh.y+=1;
            break;
        case 'h': {
                Entity* bullet = entity_new(frame, sh.x, sh.y, "--->",bullet_behaviour);
                bullet->life=60;
                // Add entities to the scene
                scene_manager_add_entity(manager, bullet);
            }
            break;
        case EOF:
            break;
    }
    return sh;
}


Shape foe_behaviour_next(Entity* e, int frame, char key) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,""};
    sh.content=strdup(e->shape->content);
    if(sh.y+foe_direction>23){
        foe_direction=-1;
    }else if(sh.y+foe_direction<1){
        foe_direction=1;
    }
    if(frame % 5 == 0){
        sh.y=sh.y+foe_direction;
    }
    return sh;
}
Shape bullet_behaviour(Entity* e, int frame, char key) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,""};
    sh.content=strdup(e->shape->content);
    if(frame % 5 == 0){
        sh.x=sh.x+1;
        e->life=e->life-1;
    }
    return sh;
}

int running=1;

void signalHandler(int signum) {
    if (running && signum == SIGINT) {
        printf(" - received SIGINT (Ctrl+C).\n");
    } else if (running && signum == SIGTERM) {
        printf(" - received SIGTERM.\n");
    }

    // Perform cleanup and exit gracefully
    // Add your cleanup code here
    running=0;
}

int main() {

    // Register signal handlers
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("Unable to register SIGINT handler");
        return 1;
    }

    if (signal(SIGTERM, signalHandler) == SIG_ERR) {
        perror("Unable to register SIGTERM handler");
        return 1;
    }

    READKEY_INIT
    int result = system("xset r rate 50 50");


    // Create a vpp buffer
    vpp=viewport_new(80,25);

    // Create a scene manager
    manager = scene_manager_new();

    // Create two entities (animated shapes) with different behaviors
    Entity* entity1 = entity_new(0, 10, 10, "HooHooHoooo",player_behaviour_next);
    Entity* entity2 = entity_new(0, 70, 10, "####",foe_behaviour_next);

    // Add entities to the scene
    scene_manager_add_entity(manager, entity1);
    scene_manager_add_entity(manager, entity2);
    // Create a shape and draw it on the vpp
    Shape* shape=shape_new(10,5,"HooHooHooo");
    // Create a shape and draw it on the vpp
    Shape* status=shape_new(0,0,":::GAME:::..........................................");

    
    int frame=0;
    unsigned int key = 0x0020;
    while(running){
        terminal_clear();
        /// free(status->content);
        shape_set_fmt(status,":::GAME::: [%c] x:%6d y:%6d,frame:%8d, objects:%2d",key, shape->x,shape->y,frame,manager->entities_count);
        // Initialize and clear the vpp buffer
        viewport_clear(vpp);
        scene_manager_update(manager, frame);
        viewport_shape_draw(vpp, status);
        scene_manager_draw_on_viewport(manager,vpp);
        // Render the vpp
        viewport_renderer(vpp);
        frame++;
        usleep(10000);
        scene_manager_remove_dead_shapes(manager);
    }
    printf(" - cleaning up \n");
    sleep(1);

    // Deallocate memory for the shape
    shape_dealloc(shape);
    shape_dealloc(status);

    // Deallocate memory for the vpp buffer
    viewport_dealloc(vpp);

    // Free resources
    scene_manager_free(manager);
    printf(" - resetting terminal \n");
    sleep(1);

    READKEY_DEINIT
    result = system("xset r rate 250 50");
    sleep(1);
    printf(" - done \n");
    sleep(1);
    return 0;
}

