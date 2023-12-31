#include "lib/lib.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>



// Create a vpp buffer
Viewport* vpp;

// Create a scene manager
SceneManager* manager;
// declare behaviours
Shape player_behaviour_next(Entity* e, int frame, char* keys);
Shape player1_behaviour_next(Entity* e, int frame, char* keys);
Shape foe_behaviour_next(Entity* e, int frame, char* keys);
Shape bullet_behaviour(Entity* e, int frame, char* keys);
Shape counter_bullet_behaviour(Entity* e, int frame, char* keys);
Shape foe_bullet_behaviour(Entity* e, int frame, char* keys);

int foe_direction=1;

int last_bullet=0;
int last_cannon=0;
int last_counter_cannon=0;
int last_move=0;

// Define two different 'next' behaviors for the shapes
Shape player_behaviour_next(Entity* e, int frame, char *keys) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);

    if(keys != NULL){

        if(frame - last_move > 1){
            if(strchr(keys,'a') != NULL){
                sh.x-=1;
                if(sh.x<0){
                    sh.x=0;
                }
                last_move=frame;
            }

            if(strchr(keys,'d') != NULL){
                sh.x+=1;
                if(sh.x>80){
                    sh.x=80;
                }
                last_move=frame;
            }

            if(strchr(keys,'w') != NULL){
                sh.y-=1;
                if(sh.y<0){
                    sh.y=0;
                }
                last_move=frame;
            }

            if(strchr(keys,'s') != NULL){
                sh.y+=1;
                if(sh.y>20){
                    sh.y=20;
                }
                last_move=frame;
            }
        }

        if(strchr(keys,'z') != NULL){
            if(frame - last_cannon > 3){
                Entity* bullet = entity_new(frame, sh.x+7, sh.y, "-==>",bullet_behaviour,5,0);
                bullet->life=200;
                bullet->power=30;
                // Add entities to the scene
                scene_manager_add_entity(manager, bullet);

                Entity* bullet2 = entity_new(frame, sh.x+7, sh.y+4, "-==>",bullet_behaviour,5,0);
                bullet2->life=200;
                bullet2->power=30;
                // Add entities to the scene
                scene_manager_add_entity(manager, bullet2);
                last_cannon=frame;
            }
        }

        if(strchr(keys,'x') != NULL){
            if(frame - last_counter_cannon > 4){
                Entity* bullet = entity_new(frame, sh.x+7, sh.y+1, "+",bullet_behaviour,2,0);
                bullet->life=80;
                bullet->power=10;
                // Add entities to the scene
                scene_manager_add_entity(manager, bullet);

                Entity* bullet2 = entity_new(frame, sh.x+7, sh.y+3, "+",bullet_behaviour,2,0);
                bullet2->life=80;
                bullet2->power=10;
                // Add entities to the scene
                scene_manager_add_entity(manager, bullet2);
                last_counter_cannon=frame;
            }
        }

        if(strchr(keys,'c') != NULL){
            if(frame - last_bullet > 1){
                Entity* bullet = entity_new(frame, sh.x+7, sh.y+2, ":",bullet_behaviour,6,0);
                bullet->life=100;
                bullet->power=5;
                // Add entities to the scene
                scene_manager_add_entity(manager, bullet);
                last_bullet=frame;
            }
        }
    }
    return sh;
}


int last_bullet1=0;
int last_cannon1=0;
int last_counter_cannon1=0;
int last_move1=0;
// Define two different 'next' behaviors for the shapes
Shape player1_behaviour_next(Entity* e, int frame, char *keys) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);

    if(keys != NULL){

        if(frame - last_move1 > 1){
            if(strchr(keys,'j') != NULL){
                sh.x-=1;
                if(sh.x<0){
                    sh.x=0;
                }
                last_move1=frame;
            }

            if(strchr(keys,'l') != NULL){
                sh.x+=1;
                if(sh.x>80){
                    sh.x=80;
                }
                last_move1=frame;
            }

            if(strchr(keys,'i') != NULL){
                sh.y-=1;
                if(sh.y<0){
                    sh.y=0;
                }
                last_move1=frame;
            }

            if(strchr(keys,'k') != NULL){
                sh.y+=1;
                if(sh.y>20){
                    sh.y=20;
                }
                last_move1=frame;
            }
        }

        if(strchr(keys,'n') != NULL){
            if(frame - last_cannon1 > 3){
                Entity* bullet = entity_new(frame, sh.x+7, sh.y, "-==>",bullet_behaviour,5,0);
                bullet->life=200;
                bullet->power=30;
                // Add entities to the scene
                scene_manager_add_entity(manager, bullet);

                Entity* bullet2 = entity_new(frame, sh.x+7, sh.y+4, "-==>",bullet_behaviour,5,0);
                bullet2->life=200;
                bullet2->power=30;
                // Add entities to the scene
                scene_manager_add_entity(manager, bullet2);
                last_cannon1=frame;
            }
        }

        if(strchr(keys,'m') != NULL){
            if(frame - last_counter_cannon1 > 4){
                Entity* bullet = entity_new(frame, sh.x+7, sh.y+1, "+",bullet_behaviour,2,0);
                bullet->life=80;
                bullet->power=10;
                // Add entities to the scene
                scene_manager_add_entity(manager, bullet);

                Entity* bullet2 = entity_new(frame, sh.x+7, sh.y+3, "+",bullet_behaviour,2,0);
                bullet2->life=80;
                bullet2->power=10;
                // Add entities to the scene
                scene_manager_add_entity(manager, bullet2);
                last_counter_cannon1=frame;
            }
        }

        if(strchr(keys,',') != NULL){
            if(frame - last_bullet1 > 1){
                Entity* bullet = entity_new(frame, sh.x+7, sh.y+2, ":",bullet_behaviour,6,0);
                bullet->life=100;
                bullet->power=5;
                // Add entities to the scene
                scene_manager_add_entity(manager, bullet);
                last_bullet1=frame;
            }
        }
    }
    return sh;
}


Shape foe_behaviour_next(Entity* e, int frame, char *keys) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);
    if(sh.y+foe_direction>20){
        foe_direction=-1;
    }else if(sh.y+foe_direction<1){
        foe_direction=1;
    }
    if(frame % 1 == 0){
        sh.y=sh.y+foe_direction;
    }
    if(frame % 5 == 0){
        Entity* rocket = entity_new(frame, sh.x-1, sh.y+2, "<==",counter_bullet_behaviour,3,0);
        rocket->life=60;
        // Add entities to the scene
        scene_manager_add_entity(manager, rocket);
    }
    return sh;
}
Shape bullet_behaviour(Entity* e, int frame, char *keys) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);
    if(frame % 1 == 0){
        sh.x=sh.x+1;
        e->life=e->life-1;
    }
    return sh;
}
Shape counter_bullet_behaviour(Entity* e, int frame, char *keys) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);
    if(frame % 1 == 0){
        sh.x=sh.x-1;
        e->life=e->life-1;
    }
    if(frame % 2 == 0){
        sh.z=!sh.z;
    }
    return sh;
}
Shape rolling_background_behaviour(Entity* e, int frame, char *keys) {
    // Handle the key input here
    Shape sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);
    if(frame % 1 == 0){
        if((sh.x-1) <-80){
            sh.x=160;
            e->color=(e->color+1)%8;
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

long long nanos() {
    struct timespec current_time;

    if (clock_gettime(CLOCK_REALTIME, &current_time) == 0) {
        long long nanoseconds = current_time.tv_sec * 1000000000LL + current_time.tv_nsec;
        return nanoseconds;
    } else {
        return -1;
    }
}
int main(int argc,const char **argv) {
    printf("\ngot %d arguments\n",argc);

    if(argc < 2 ) {
        printf("please specify an input device from /dev/input/event*\n");
        return -1;
    }
    //sleep(1);

    // Create a Keyboard object
    Keyboard *keyboard = keyboard_new(argv[1]);
    //sleep(1);
    if (keyboard == NULL) {
        fprintf(stderr, "Failed to initialize Keyboard\n");
        return 1;
    }

    // Register signal handlers
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("Unable to register SIGINT handler");
        return 1;
    }

    if (signal(SIGTERM, signalHandler) == SIG_ERR) {
        perror("Unable to register SIGTERM handler");
        return 1;
    }
    // int result = system("xset r rate 50 50");


    // Create a vpp buffer
    vpp=viewport_new(160,25);

    // Create a scene manager
    manager = scene_manager_new();

    // Create two entities (animated shapes) with different behaviors
    Entity* bkg0 = entity_new(0, 160, 0, background,rolling_background_behaviour,2,1);
    bkg0->shape->z=-1;
    bkg0->collision=0;
    bkg0->power=0;
    Entity* bkg1 = entity_new(0, 80, 0, background,rolling_background_behaviour,2,1);
    bkg1->shape->z=-1;
    bkg1->collision=0;
    bkg1->power=0;
    Entity* bkg2 = entity_new(0, 0, 0, background,rolling_background_behaviour,2,1);
    bkg2->shape->z=-1;
    bkg2->collision=0;
    bkg2->power=0;
    Entity* player = entity_new(0, 10, 10, ship_shape,player_behaviour_next,4,1);
    player->power=10;
    player->life=1000;
    Entity* player1 = entity_new(0, 10, 10, ship_shape,player1_behaviour_next,5,1);
    bkg2->shape->z=1;
    player1->power=10;
    player1->life=1000;
    Entity* baddie = entity_new(0, 140, 10, foe_shape,foe_behaviour_next,1,1);
    baddie->power=100;
    baddie->life=1000;

    // Add entities to the scene
    scene_manager_add_entity(manager, bkg0);
    scene_manager_add_entity(manager, bkg1);
    scene_manager_add_entity(manager, bkg2);
    scene_manager_add_entity(manager, player);
    scene_manager_add_entity(manager, player1);
    scene_manager_add_entity(manager, baddie);
    // Create a shape and draw it on the vpp
    Shape* shape=shape_new(10,5,"HooHooHooo");
    // Create a shape and draw it on the vpp
    Shape* status=shape_new(0,0,":::GAME:::.🚀🚀🚀......................................");

    printf(" starting \n");
    for(int k=0;k<manager->entities_count;k++){
        entity__print(manager->entities[k]);
    }
    printf(" \n");
    for(int s=10;s>0 && running;s--){
        printf("\rstarting in %2d seconds",s/2);
        fflush(stdout);
        usleep(500);
        char *pressed = keyboard_get_pressed(keyboard);
        if(pressed != NULL) {
            free(pressed);
            break;
        }
    }
    int frame=0;
    double fps;
    long long duration;
    long long t0=nanos()/1000;
    long long last_update=t0;
    while(running){
        long long d0=nanos()/1000;

        char *pressed = keyboard_get_pressed(keyboard);
        if(0 || d0-last_update > 40000){
            terminal_clear();
            shape_set_fmt(status,":::GAME::: x:%6d y:%6d,frame:%8d, objects:%2d, d: %lld, fpns: %lld", shape->x,shape->y,frame,manager->entities_count,duration,fps);
            // Initialize and clear the vpp buffer
            viewport_clear(vpp);
            scene_manager_update(manager, frame,pressed);
            viewport_shape_draw(vpp, status,3,0);
            scene_manager_draw_on_viewport(manager,vpp);
            // Render the vpp
            viewport_renderer(vpp);
            scene_manager_do_collisions(manager);
            scene_manager_remove_dead_shapes(manager);
            last_update=d0;
            long long t1 = nanos()/1000;
            duration=t1-d0;
            fps=frame/(t1-t0);
            frame++;
        }
        usleep(1000);

        if(pressed != NULL){
            free(pressed);
        }
    }
    printf(" final score \n");
    for(int k=0;k<manager->entities_count;k++){
        entity__print(manager->entities[k]);
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

    // Clean up and exit
    keyboard_deinit(keyboard);
    sleep(1);
    printf(" - done \n");
    sleep(1);
    return 0;
}

