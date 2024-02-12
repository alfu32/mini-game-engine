#ifndef usedynlib
    #include "ohmygame/include/ohmygame.h"
#else
    #include <ohmygame/ohmygame.h>
#endif
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
    #include "ohmygame-demo-lib.h"



// Create a vpp buffer
viewport_t* vpp;

// Create a scene manager
scene_manager_t* manager;
// declare behaviours
shape_t player_behaviour_next(entity_t* e, int frame, keyboard_t* kb);
shape_t player1_behaviour_next(entity_t* e, int frame, keyboard_t* kb);
shape_t foe_behaviour_next(entity_t* e, int frame, keyboard_t* kb);
shape_t bullet_behaviour(entity_t* e, int frame, keyboard_t* kb);
shape_t counter_bullet_behaviour(entity_t* e, int frame, keyboard_t* kb);
shape_t foe_bullet_behaviour(entity_t* e, int frame, keyboard_t* kb);
entity_t * baddie_new();


int foe_direction=1;

int last_bullet[10];
int last_cannon[10];
int last_counter_cannon[10];
int last_move[10];

int try_move_ent_x(entity_t* e,int x,int frame,int last_move,keyboard_t* kb,char key,int clamp_value_min,int clamp_value_max){

    if(frame - last_move > 1){
        e->shape->x += x;
        if(e->shape->x < clamp_value_min) {
            e->shape->x = clamp_value_min;
        }

        if(e->shape->x > clamp_value_max) {
            e->shape->x = clamp_value_max;
        }
        return frame;
    } else {
        return last_move;
    }
}

// Define two different 'next' behaviors for the shapes
shape_t player_behaviour_next(entity_t* e, int frame, keyboard_t *kb) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);

    if(kb != NULL){
        rectangle_t* shr = shape__get_bounding_client_rect(e->shape);
        int shrw=shr->width;
        free(shr);

        if(frame - last_move[0] > 1) {
            BEHAVIOUR_MOVE_LEFT(sh.x,0,kb, KEY_A,last_move[0])
            BEHAVIOUR_MOVE_RIGHT(sh.x,vpp->width-20,kb, KEY_D,last_move[0])
            BEHAVIOUR_MOVE_LEFT(sh.y,0,kb, KEY_W, last_move[0])
            BEHAVIOUR_MOVE_RIGHT(sh.y,vpp->height-5,kb, KEY_S, last_move[0])
        }

        int dud;
        dud=last_cannon[0];
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y-1, kb, KEY_Z,200,30,1001,dud,bullet_behaviour)
        dud=last_cannon[0];
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y, kb, KEY_Z,200,30,1001,dud,bullet_behaviour)
        dud=last_cannon[0];
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y+4, kb, KEY_Z,200,30,1001,dud,bullet_behaviour)
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y+5, kb, KEY_Z,200,30,1001,last_cannon[0],bullet_behaviour)

        dud=last_counter_cannon[0];
        BEHAVIOUR_FIRE_PROJECTILE( "+" , sh.x+shrw, sh.y+1, kb, KEY_X,200,30,1001,dud,bullet_behaviour)
        BEHAVIOUR_FIRE_PROJECTILE( "+" , sh.x+shrw, sh.y+3, kb, KEY_X,200,30,1001,last_counter_cannon[0],bullet_behaviour)

        BEHAVIOUR_FIRE_PROJECTILE( "+" , sh.x+shrw, sh.y+2, kb, KEY_C,200,30,1001,last_bullet[0],bullet_behaviour)
    }
    return sh;
}

// Define two different 'next' behaviors for the shapes
shape_t player1_behaviour_next(entity_t* e, int frame, keyboard_t *kb) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);

    if(kb != NULL){
        rectangle_t* shr = shape__get_bounding_client_rect(e->shape);
        int shrw=shr->width;
        free(shr);

        if(frame - last_move[1] > 1){
            BEHAVIOUR_MOVE_LEFT(sh.x,0,kb, KEY_J,last_move[1])
            BEHAVIOUR_MOVE_RIGHT(sh.x,vpp->width-20,kb, KEY_L,last_move[1])
            BEHAVIOUR_MOVE_LEFT(sh.y,0,kb, KEY_I, last_move[1])
            BEHAVIOUR_MOVE_RIGHT(sh.y,vpp->height-5,kb, KEY_K, last_move[1])
        }
        int dud;
        dud=last_cannon[1];
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y-1, kb, KEY_N,200,30,1001,dud,bullet_behaviour)
        dud=last_cannon[1];
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y, kb, KEY_N,200,30,1001,dud,bullet_behaviour)
        dud=last_cannon[1];
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y+4, kb, KEY_N,200,30,1001,dud,bullet_behaviour)
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y+5, kb, KEY_N,200,30,1001,last_cannon[1],bullet_behaviour)

        dud=last_counter_cannon[1];
        BEHAVIOUR_FIRE_PROJECTILE( "+" , sh.x+shrw, sh.y+1, kb, KEY_M,200,30,1001,dud,bullet_behaviour)
        BEHAVIOUR_FIRE_PROJECTILE( "+" , sh.x+shrw, sh.y+3, kb, KEY_M,200,30,1001,last_counter_cannon[1],bullet_behaviour)

        BEHAVIOUR_FIRE_PROJECTILE( "+" , sh.x+shrw, sh.y+2, kb, KEY_COMMA,200,30,1001,last_bullet[1],bullet_behaviour)
    }
    return sh;
}

// Define two different 'next' behaviors for the shapes
shape_t player2_behaviour_next(entity_t* e, int frame, keyboard_t *kb) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);

    if(kb != NULL) {
        rectangle_t* shr = shape__get_bounding_client_rect(e->shape);
        int shrw=shr->width;
        free(shr);

        if(frame - last_move[2] > 1) {
            BEHAVIOUR_MOVE_LEFT(sh.x,0,kb, KEY_LEFT,last_move[2])
            BEHAVIOUR_MOVE_RIGHT(sh.x,vpp->width-20,kb, KEY_RIGHT,last_move[2])
            BEHAVIOUR_MOVE_LEFT(sh.y,0,kb, KEY_UP, last_move[2])
            BEHAVIOUR_MOVE_RIGHT(sh.y,vpp->height-5,kb, KEY_DOWN, last_move[2])
        }
        int dud;
        dud=last_cannon[2];
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y-1, kb, KEY_HOME,200,30,1001,dud,bullet_behaviour)
        dud=last_cannon[2];
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y, kb, KEY_HOME,200,30,1001,dud,bullet_behaviour)
        dud=last_cannon[2];
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y+4, kb, KEY_HOME,200,30,1001,dud,bullet_behaviour)
        BEHAVIOUR_FIRE_PROJECTILE( "-==>" , sh.x+shrw, sh.y+5, kb, KEY_HOME,200,30,1001,last_cannon[2],bullet_behaviour)

        dud=last_counter_cannon[2];
        BEHAVIOUR_FIRE_PROJECTILE( "+" , sh.x+shrw, sh.y+1, kb, KEY_END,200,30,1001,dud,bullet_behaviour)
        BEHAVIOUR_FIRE_PROJECTILE( "+" , sh.x+shrw, sh.y+3, kb, KEY_END,200,30,1001,last_counter_cannon[2],bullet_behaviour)

        BEHAVIOUR_FIRE_PROJECTILE( "+" , sh.x+shrw, sh.y+2, kb, KEY_PAGEUP,200,30,1001,last_bullet[2],bullet_behaviour)
    }
    return sh;
}


shape_t foe_behaviour_next(entity_t* e, int frame, keyboard_t *kb) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);
    if(sh.y+foe_direction>vpp->height-5){
        foe_direction=-1;
    }else if(sh.y+foe_direction<1){
        foe_direction=1;
    }
    if(frame % 1 == 0){
        sh.y=sh.y+foe_direction;
    }
    if(frame % 5 == 0){
        entity_t* rocket = entity__new(frame, sh.x-5, sh.y-1, "<==",counter_bullet_behaviour,3,0);
        rocket->shape->z=0;
        rocket->life=60;
        rocket->power=30;
        rocket->team=1001;
        // Add entities to the scene
        scene_manager__add_entity(manager, rocket);


        entity_t* rocket1 = entity__new(frame, sh.x-5, sh.y+5, "<==",counter_bullet_behaviour,3,0);
        rocket1->shape->z=1;
        rocket1->life=60;
        rocket1->power=30;
        rocket1->team=1001;
        // Add entities to the scene
        scene_manager__add_entity(manager, rocket1);
    }
    return sh;
}
shape_t bullet_behaviour(entity_t* e, int frame, keyboard_t *kb) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);
    if(frame % 1 == 0){
        sh.x=sh.x+1;
        e->life=e->life-1;
    }
    return sh;
}
shape_t counter_bullet_behaviour(entity_t* e, int frame, keyboard_t *kb) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);
    if(frame % 1 == 0){
        sh.x=sh.x-2;
        e->life=e->life-2;
    }
    if(frame % 2 == 0){
        sh.z=!sh.z;
    }
    return sh;
}
shape_t rolling_background_behaviour(entity_t* e, int frame, keyboard_t *kb) {
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
               *                                                                \n\
        *                         .                                             \n\
                *                |                    *                         \n\
         *           *          **                                              \n\
                                **                                              \n\
                         *      **                                              \n\
          .                    +--+                                             \n\
                               |  |                                             \n\
                               |  |                               .             \n\
                               |  |          *                                  \n\
 .           ======            |  |                          +-------+          \n\
         +----------+          |  |                      +---|     # |          \n\
         | ###_____ |        +-+--+-+      .             |   |  #    |          \n\
         | _#__#___ |        |      |                    |   |       |          \n\
         | ____#___ |        |  #   |                    |   |     # |          \n\
         | #______# |        |      |                    +---+-------+    ---   \n\
         | ___#____ |        |      |                    |           |   /   \\  \n\
         | ___#_#_# |        |      |        ======================  |  |     | \n\
    +------+_______ |      +-+------+-+         _+--------------+    |  |     | \n\
    |      |_______ |      |   #      |         _| ____________ |    |   \\   /  \n\
    |      |_______ |      |       #  |         _|              |    |    -+-   \n\
    |      |_______ |      |          |     _____| ____________ |    |     |    \n\
    |      |        |      |          |          |              |    |     |    \n\
=======================__================___#__============================|====\n\
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

const char* enterprise="\
             _____________\n\
          === NCC 01 D ======\n\
           ============\n\
  _____     ###### \n\
 ========###### \n\
 \n\
";
const char* figure_gaby="\
GABIGABIGABY\n\
GA =*  =* BY\n\
GA    |   BY\n\
GA  -=_=- BY\n\
GA        BY\n\
GABIGABIGABY\n\
";

entity_t * baddie_new(){
    entity_t *baddie = entity__new(0, 140, 5, foe_shape, foe_behaviour_next, 1, 1);
    baddie->shape->z = 0;
    baddie->power = 100;
    baddie->life = 1000;
    baddie->team = 1000;
    return baddie;
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
    vpp=viewport__new(180,40);

    // Create a scene manager
    manager = scene_manager__new();

    // Create two entities (animated shapes) with different behaviors
    entity_t* bkg0 = entity__new(0, 180, 12, background,rolling_background_behaviour,2,1);
    bkg0->shape->z=-1;
    bkg0->collision=0;
    bkg0->power=0;
    bkg0->team=1;
    entity_t* bkg1 = entity__new(0, 90, 12, background,rolling_background_behaviour,2,1);
    bkg1->shape->z=-1;
    bkg1->collision=0;
    bkg1->power=0;
    bkg1->team=1;
    entity_t* bkg2 = entity__new(0, 0, 12, background,rolling_background_behaviour,2,1);
    bkg2->shape->z=-1;
    bkg2->collision=0;
    bkg2->power=0;
    bkg2->team=1;
    entity_t* player = entity__new(0, 10, 10, enterprise,player_behaviour_next,4,1);
    player->shape->z=0;
    player->power=10;
    player->life=1000;
    player->team=1000;
    entity_t* player1 = entity__new(0, 10, 22, ship_shape,player1_behaviour_next,5,1);
    player1->shape->z=1;
    player1->power=10;
    player1->life=1000;
    player1->team=1000;
    entity_t* player2 = entity__new(0, 10, 36, figure_gaby,player2_behaviour_next,5,1);
    player1->shape->z=1;
    player1->power=10;
    player1->life=1000;
    player1->team=1000;
    entity_t *baddie = baddie_new();

    // Add entities to the scene
    scene_manager__add_entity(manager, bkg0);
    scene_manager__add_entity(manager, bkg1);
    scene_manager__add_entity(manager, bkg2);
    scene_manager__add_entity(manager, player);
    scene_manager__add_entity(manager, player1);
    scene_manager__add_entity(manager, player2);
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
        usleep(2000);
    for(int s=10;s>0 && running;s--){
        keyboard__refresh(keyboard);
        printf("\rstarting in %2d seconds; keyboard: ",s/2);
        for(int i=0;i<KEY_MAX;i++){
            printf("%d,",keyboard->pressed[i]);
        }
        fflush(stdout);
        usleep(2000);
    }
    int frame=0;
    double fps;
    long long duration;
    long long t0=micros();
    long long last_update=t0;
    int create_new_baddie=0;
    while(running){
        long long d0=micros();

        keyboard__refresh(keyboard);
        if(0 || d0-last_update > 40000){
            terminal_clear();
            shape__set_content_fmt(status,":::GAME::: x:%6d y:%6d,frame:%8d, objects:%2d, d: %lld, fpns: %lld", shape->x,shape->y,frame,manager->entities_count,duration,fps);
            // Initialize and clear the vpp buffer
            viewport__clear(vpp);
            scene_manager__update(manager, frame,keyboard);
            viewport_shape_draw(vpp, status,3,0);
            scene_manager__draw_on_viewport(manager,vpp);
            // Render the vpp
            viewport__renderer(vpp);
            scene_manager__do_collisions(manager);
            if(baddie->life<=0){
                create_new_baddie=1;
            }
            scene_manager__remove_dead_shapes(manager);
            last_update=d0;
            long long t1 = micros();
            duration=t1-d0;
            fps=frame/(t1-t0);
            frame++;

            if(create_new_baddie){
                create_new_baddie=0;
                baddie=baddie_new();
                scene_manager__add_entity(manager, baddie);
                entity_t* baddie2=baddie_new();
                scene_manager__add_entity(manager, baddie2);
                baddie2->shape->y=15;
            }
        }
        usleep(500);
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
