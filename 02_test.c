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
viewport_t* vpp;

// Create a scene manager
scene_manager_t* manager;
// declare behaviours
shape_t player_behaviour_next(entity_t* e, int frame, char* keys);
shape_t player1_behaviour_next(entity_t* e, int frame, char* keys);
shape_t foe_behaviour_next(entity_t* e, int frame, char* keys);
shape_t bullet_behaviour(entity_t* e, int frame, char* keys);
shape_t counter_bullet_behaviour(entity_t* e, int frame, char* keys);
shape_t foe_bullet_behaviour(entity_t* e, int frame, char* keys);

int foe_direction=1;

int last_bullet=0;
int last_cannon=0;
int last_counter_cannon=0;
int last_move=0;

// Define two different 'next' behaviors for the shapes
shape_t player_behaviour_next(entity_t* e, int frame, char *keys) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
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
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y, "-==>",bullet_behaviour,5,0);
                bullet->life=200;
                bullet->power=30;
                bullet->team=1000;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet);

                entity_t* bullet2 = entity__new(frame, sh.x+7, sh.y+4, "-==>",bullet_behaviour,5,0);
                bullet2->life=200;
                bullet2->power=30;
                bullet2->team=1000;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet2);
                last_cannon=frame;
            }
        }

        if(strchr(keys,'x') != NULL){
            if(frame - last_counter_cannon > 4){
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y+1, "+",bullet_behaviour,2,0);
                bullet->life=80;
                bullet->power=10;
                bullet->team=1000;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet);

                entity_t* bullet2 = entity__new(frame, sh.x+7, sh.y+3, "+",bullet_behaviour,2,0);
                bullet2->life=80;
                bullet2->power=10;
                bullet2->team=1000;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet2);
                last_counter_cannon=frame;
            }
        }

        if(strchr(keys,'c') != NULL){
            if(frame - last_bullet > 1){
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y+2, ":",bullet_behaviour,6,0);
                bullet->life=100;
                bullet->power=5;
                bullet->team=1000;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet);
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
shape_t player1_behaviour_next(entity_t* e, int frame, char *keys) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
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
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y, "-==>",bullet_behaviour,5,0);
                bullet->life=200;
                bullet->power=30;
                bullet->team=1000;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet);

                entity_t* bullet2 = entity__new(frame, sh.x+7, sh.y+4, "-==>",bullet_behaviour,5,0);
                bullet2->life=200;
                bullet2->power=30;
                bullet2->team=1000;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet2);
                last_cannon1=frame;
            }
        }

        if(strchr(keys,'m') != NULL){
            if(frame - last_counter_cannon1 > 4){
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y+1, "+",bullet_behaviour,2,0);
                bullet->life=80;
                bullet->power=10;
                bullet->team=1000;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet);

                entity_t* bullet2 = entity__new(frame, sh.x+7, sh.y+3, "+",bullet_behaviour,2,0);
                bullet2->life=80;
                bullet2->power=10;
                bullet2->team=1000;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet2);
                last_counter_cannon1=frame;
            }
        }

        if(strchr(keys,',') != NULL){
            if(frame - last_bullet1 > 1){
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y+2, ":",bullet_behaviour,6,0);
                bullet->life=100;
                bullet->power=5;
                bullet->team=1000;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet);
                last_bullet1=frame;
            }
        }
    }
    return sh;
}


shape_t foe_behaviour_next(entity_t* e, int frame, char *keys) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
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
        entity_t* rocket = entity__new(frame, sh.x-1, sh.y+2, "<==",counter_bullet_behaviour,3,0);
        rocket->life=60;
        rocket->power=30;
        rocket->team=1001;
        // Add entities to the scene
        scene_manager__add_entity(manager, rocket);
    }
    return sh;
}
shape_t bullet_behaviour(entity_t* e, int frame, char *keys) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);
    if(frame % 1 == 0){
        sh.x=sh.x+1;
        e->life=e->life-1;
    }
    return sh;
}
shape_t counter_bullet_behaviour(entity_t* e, int frame, char *keys) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
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
shape_t rolling_background_behaviour(entity_t* e, int frame, char *keys) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
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
        *                         .                                    \n\
                *                |                    *                \n\
         *           *          **                                     \n\
                                **                                     \n\
                         *      **                                     \n\
          .                    +--+                                    \n\
                               |  |                                    \n\
                               |  |                               .    \n\
                               |  |          *                         \n\
 .           ======            |  |                          +-------+ \n\
         +----------+          |  |                      +---|     # | \n\
         | ###_____ |        +-+--+-+      .             |   |  #    | \n\
         | _#__#___ |        |      |                    |   |       | \n\
         | ____#___ |        |  #   |                    |   |     # | \n\
         | #______# |        |      |                    +---+-------+ \n\
         | ___#____ |        |      |                    |           | \n\
         | ___#_#_# |        |      |        ======================  | \n\
    +------+_______ |      +-+------+-+         _+--------------+    | \n\
    |      |_______ |      |   #      |         _| ____________ |    | \n\
    |      |_______ |      |       #  |         _|              |    | \n\
    |      |_______ |      |          |     _____| ____________ |    | \n\
    |      |        |      |          |          |              |    | \n\
=======================__================___#__========================\n\
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

    // Create a keyboard_t object
    keyboard_t *keyboard = keyboard__new(argv[1]);
    //sleep(1);
    if (keyboard == NULL) {
        fprintf(stderr, "Failed to initialize keyboard_t\n");
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
    vpp=viewport__new(160,25);

    // Create a scene manager
    manager = scene_manager__new();

    // Create two entities (animated shapes) with different behaviors
    entity_t* bkg0 = entity__new(0, 160, 0, background,rolling_background_behaviour,2,1);
    bkg0->shape->z=-1;
    bkg0->collision=0;
    bkg0->power=0;
    bkg0->team=1;
    entity_t* bkg1 = entity__new(0, 80, 0, background,rolling_background_behaviour,2,1);
    bkg1->shape->z=-1;
    bkg1->collision=0;
    bkg1->power=0;
    bkg1->team=1;
    entity_t* bkg2 = entity__new(0, 0, 0, background,rolling_background_behaviour,2,1);
    bkg2->shape->z=-1;
    bkg2->collision=0;
    bkg2->power=0;
    bkg2->team=1;
    entity_t* player = entity__new(0, 10, 10, ship_shape,player_behaviour_next,4,1);
    player->shape->z=0;
    player->power=10;
    player->life=1000;
    player->team=1000;
    entity_t* player1 = entity__new(0, 10, 10, ship_shape,player1_behaviour_next,5,1);
    player1->shape->z=1;
    player1->power=10;
    player1->life=1000;
    player1->team=1000;
    entity_t* baddie = entity__new(0, 140, 10, foe_shape,foe_behaviour_next,1,1);
    baddie->shape->z=0;
    baddie->power=100;
    baddie->life=1000;
    baddie->team=1001;

    // Add entities to the scene
    scene_manager__add_entity(manager, bkg0);
    scene_manager__add_entity(manager, bkg1);
    scene_manager__add_entity(manager, bkg2);
    scene_manager__add_entity(manager, player);
    scene_manager__add_entity(manager, player1);
    scene_manager__add_entity(manager, baddie);
    // Create a shape and draw it on the vpp
    shape_t* shape=shape__new(10,5,"HooHooHooo");
    // Create a shape and draw it on the vpp
    shape_t* status=shape__new(0,0,":::GAME:::.🚀🚀🚀......................................");

    printf(" starting \n");
    for(int k=0;k<manager->entities_count;k++){
        entity__print(manager->entities[k]);
    }
    printf(" \n");
    for(int s=10;s>0 && running;s--){
        printf("\rstarting in %2d seconds",s/2);
        fflush(stdout);
        usleep(500);
        char *pressed = keyboard__fetch_pressed(keyboard);
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

        char *pressed = keyboard__fetch_pressed(keyboard);
        if(0 || d0-last_update > 40000){
            terminal_clear();
            shape__set_content_fmt(status,":::GAME::: x:%6d y:%6d,frame:%8d, objects:%2d, d: %lld, fpns: %lld", shape->x,shape->y,frame,manager->entities_count,duration,fps);
            // Initialize and clear the vpp buffer
            viewport__clear(vpp);
            scene_manager__update(manager, frame,pressed);
            viewport_shape_draw(vpp, status,3,0);
            scene_manager__draw_on_viewport(manager,vpp);
            // Render the vpp
            viewport__renderer(vpp);
            scene_manager__do_collisions(manager);
            scene_manager__remove_dead_shapes(manager);
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
    shape__dealloc(shape);
    shape__dealloc(status);

    // Deallocate memory for the vpp buffer
    viewport__dealloc(vpp);

    // Free resources
    scene_manager__free(manager);
    printf(" - resetting terminal \n");
    sleep(1);

    // Clean up and exit
    keyboard__deinit(keyboard);
    sleep(1);
    printf(" - done \n");
    sleep(1);
    return 0;
}

