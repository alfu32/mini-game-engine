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
Shape player_behaviour_next(Entity* e, int frame, char* keys);
Shape foe_behaviour_next(Entity* e, int frame, char* keys);
Shape bullet_behaviour(Entity* e, int frame, char* keys);
Shape foe_bullet_behaviour(Entity* e, int frame, char* keys);

int foe_direction=1;

int last_bullet=0;
int last_cannon=0;

// Define two different 'next' behaviors for the shapes
Shape player_behaviour_next(Entity* e, int frame, char *keys) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,""};
    sh.content=strdup(e->shape->content);

    if(strchr(keys,'z') != NULL){
        sh.x-=1;
    }

    if(strchr(keys,'c') != NULL){
        sh.x+=1;
    }

    if(strchr(keys,'s') != NULL){
        sh.y-=1;
    }

    if(strchr(keys,'x') != NULL){
        sh.y+=1;
    }

    if(strchr(keys,'g') != NULL){
        if(frame - last_cannon > 50){
            Entity* bullet = entity_new(frame, sh.x+3, sh.y, "-==>",bullet_behaviour,5,0);
            bullet->life=60;
            // Add entities to the scene
            scene_manager_add_entity(manager, bullet);




            Entity* bullet2 = entity_new(frame, sh.x+3, sh.y+4, "-==>",bullet_behaviour,5,0);
            bullet2->life=60;
            // Add entities to the scene
            scene_manager_add_entity(manager, bullet2);
            last_cannon=frame;
        }
    }

    if(strchr(keys,'h') != NULL){
        if(frame - last_bullet > 10){
            Entity* bullet = entity_new(frame, sh.x+5, sh.y+2, ":",bullet_behaviour,6,0);
            bullet->life=60;
            // Add entities to the scene
            scene_manager_add_entity(manager, bullet);
            last_bullet=frame;
        }
    }
    return sh;
}


Shape foe_behaviour_next(Entity* e, int frame, char *keys) {
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
Shape bullet_behaviour(Entity* e, int frame, char *keys) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,""};
    sh.content=strdup(e->shape->content);
    if(frame % 5 == 0){
        sh.x=sh.x+1;
        e->life=e->life-1;
    }
    return sh;
}
Shape rolling_background_behaviour(Entity* e, int frame, char *keys) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,""};
    sh.content=strdup(e->shape->content);
    if(frame % 5 == 0){
        if((sh.x-1) <-80){
            sh.x=80;
        } else {
            sh.x=sh.x-1;
        }
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
const char* background="\
                                                                       \n\
               *                                                       \n\
        *                                                              \n\
                *                                     *                \n\
         *           *          **                                     \n\
                                **                                     \n\
                         *      **                                     \n\
                               +--+                                    \n\
                               |  |                                    \n\
                               |  |                                    \n\
                               |  |          *                         \n\
             ======            |  |                          +-------+ \n\
         +----------+          |  |                      +---|       | \n\
         | ________ |        +-+--+-+                    |   |       | \n\
         | ________ |        |      |                    |   |       | \n\
         | ________ |        |      |                    |   |       | \n\
         | ________ |        |      |                    +---+-------+ \n\
         | ________ |        |      |                    |           | \n\
         | ________ |        |      |        ======================  | \n\
    +------+_______ |      +-+------+-+         _+--------------+    | \n\
    |      |_______ |      |          |         _| ____________ |    | \n\
    |      |_______ |      |          |         _|              |    | \n\
    |      |_______ |      |          |     _____| ____________ |    | \n\
    |      |        |      |          |          |              |    | \n\
=======================__================______========================\n\
";
const char* ship_shape="\
  \\-\n\
  \\\n\
#====>\n\
  /\n\
 /-\n\
";

const char* foe_shape="\
   ##\n\
 -####\n\
=#######\n\
 -####\n\
  ##\n\
";
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
    Entity* bkg0 = entity_new(0, 80, 0, background,rolling_background_behaviour,2,1);
    Entity* bkg1 = entity_new(0, 0, 0, background,rolling_background_behaviour,2,1);
    Entity* entity1 = entity_new(0, 10, 10, ship_shape,player_behaviour_next,4,1);
    Entity* entity2 = entity_new(0, 70, 10, foe_shape,foe_behaviour_next,1,1);

    // Add entities to the scene
    scene_manager_add_entity(manager, bkg0);
    scene_manager_add_entity(manager, bkg1);
    scene_manager_add_entity(manager, entity1);
    scene_manager_add_entity(manager, entity2);
    // Create a shape and draw it on the vpp
    Shape* shape=shape_new(10,5,"HooHooHooo");
    // Create a shape and draw it on the vpp
    Shape* status=shape_new(0,0,":::GAME:::.🚀🚀🚀......................................");

    
    int frame=0;
    while(running){


        char *pressed = keys_get_pressed();
        terminal_clear();
        /// free(status->content);
        shape_set_fmt(status,":::GAME::: [%s] x:%6d y:%6d,frame:%8d, objects:%2d",pressed, shape->x,shape->y,frame,manager->entities_count);
        // Initialize and clear the vpp buffer
        viewport_clear(vpp);
        scene_manager_update(manager, frame,pressed);
        viewport_shape_draw(vpp, status,3,0);
        scene_manager_draw_on_viewport(manager,vpp);
        // Render the vpp
        viewport_renderer(vpp);
        frame++;
        usleep(5*1000);
        scene_manager_remove_dead_shapes(manager);

        free(pressed);
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

