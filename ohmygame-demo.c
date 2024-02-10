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

int last_bullet=0;
int last_cannon=0;
int last_counter_cannon=0;
int last_move=0;

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
        /// last_move=try_move_ent_x(e,-1,frame,last_move,keys,'a',0,140);
        if(frame - last_move > 1){
            last_move=try_move_ent_x(e,-1,frame,last_move,kb,'a',0,140);

            if(keyboard__contains_key_code(kb,KEY_A) != 0){
                sh.x-=1;
                if(sh.x<0){
                    sh.x=0;
                }
                last_move=frame;
            }

            if(keyboard__contains_key_code(kb,KEY_D) != 0){
                sh.x+=1;
                if(sh.x>vpp->width-20){
                    sh.x=vpp->width-20;
                }
                last_move=frame;
            }

            if(keyboard__contains_key_code(kb,KEY_W) != 0){
                sh.y-=1;
                if(sh.y<0){
                    sh.y=0;
                }
                last_move=frame;
            }

            if(keyboard__contains_key_code(kb,KEY_S) != 0){
                sh.y+=1;
                if(sh.y>vpp->height-5){
                    sh.y=vpp->height-5;
                }
                last_move=frame;
            }
        }

        if(keyboard__contains_key_code(kb,KEY_Z) != 0){
            if(frame - last_cannon > 3){
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y, "-==>",bullet_behaviour,5,0);
                bullet->life=200;
                bullet->power=30;
                bullet->team=1001;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet);

                entity_t* bullet2 = entity__new(frame, sh.x+7, sh.y+4, "-==>",bullet_behaviour,5,0);
                bullet2->life=200;
                bullet2->power=30;
                bullet2->team=1001;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet2);
                last_cannon=frame;
            }
        }

        if(keyboard__contains_key_code(kb,KEY_Z) != 0){
            if(frame - last_counter_cannon > 4){
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y+1, "+",bullet_behaviour,2,0);
                bullet->life=80;
                bullet->power=10;
                bullet->team=1001;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet);

                entity_t* bullet2 = entity__new(frame, sh.x+7, sh.y+3, "+",bullet_behaviour,2,0);
                bullet2->life=80;
                bullet2->power=10;
                bullet2->team=1001;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet2);
                last_counter_cannon=frame;
            }
        }

        if(keyboard__contains_key_code(kb,KEY_C) != 0){
            if(frame - last_bullet > 1){
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y+2, ":",bullet_behaviour,6,0);
                bullet->life=100;
                bullet->power=5;
                bullet->team=1001;
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
shape_t player1_behaviour_next(entity_t* e, int frame, keyboard_t *kb) {
    // Handle the key input here
    shape_t sh = {e->shape->x,e->shape->y,e->shape->z,""};
    sh.content=strdup(e->shape->content);

    if(kb != NULL){

        if(frame - last_move1 > 1){
            if(keyboard__contains_key_code(kb,KEY_J) != 0){
                sh.x-=1;
                if(sh.x<0){
                    sh.x=0;
                }
                last_move1=frame;
            }

            if(keyboard__contains_key_code(kb,KEY_L) != 0){
                sh.x+=1;
                if(sh.x>vpp->width-20){
                    sh.x=vpp->width-20;
                }
                last_move1=frame;
            }

            if(keyboard__contains_key_code(kb,KEY_I) != 0){
                sh.y-=1;
                if(sh.y<0){
                    sh.y=0;
                }
                last_move1=frame;
            }

            if(keyboard__contains_key_code(kb,KEY_K) != 0){
                sh.y+=1;
                if(sh.y>vpp->height-5){
                    sh.y=vpp->height-5;
                }
                last_move1=frame;
            }
        }

        if(keyboard__contains_key_code(kb,KEY_N) != 0){
            if(frame - last_cannon1 > 3){
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y, "-==>",bullet_behaviour,5,0);
                bullet->life=200;
                bullet->power=30;
                bullet->team=1001;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet);

                entity_t* bullet2 = entity__new(frame, sh.x+7, sh.y+4, "-==>",bullet_behaviour,5,0);
                bullet2->life=200;
                bullet2->power=30;
                bullet2->team=1001;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet2);
                last_cannon1=frame;
            }
        }

        if(keyboard__contains_key_code(kb,KEY_M) != 0){
            if(frame - last_counter_cannon1 > 4){
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y+1, "+",bullet_behaviour,2,0);
                bullet->life=80;
                bullet->power=10;
                bullet->team=1001;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet);

                entity_t* bullet2 = entity__new(frame, sh.x+7, sh.y+3, "+",bullet_behaviour,2,0);
                bullet2->life=80;
                bullet2->power=10;
                bullet2->team=1001;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet2);
                last_counter_cannon1=frame;
            }
        }

        if(keyboard__contains_key_code(kb,KEY_COMMA) != 0){
            if(frame - last_bullet1 > 1){
                entity_t* bullet = entity__new(frame, sh.x+7, sh.y+2, ":",bullet_behaviour,6,0);
                bullet->life=100;
                bullet->power=5;
                bullet->team=1001;
                // Add entities to the scene
                scene_manager__add_entity(manager, bullet);
                last_bullet1=frame;
            }
        }
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
    vpp=viewport__new(160,40);

    // Create a scene manager
    manager = scene_manager__new();

    // Create two entities (animated shapes) with different behaviors
    entity_t* bkg0 = entity__new(0, 160, 18, background,rolling_background_behaviour,2,1);
    bkg0->shape->z=-1;
    bkg0->collision=0;
    bkg0->power=0;
    bkg0->team=1;
    entity_t* bkg1 = entity__new(0, 80, 18, background,rolling_background_behaviour,2,1);
    bkg1->shape->z=-1;
    bkg1->collision=0;
    bkg1->power=0;
    bkg1->team=1;
    entity_t* bkg2 = entity__new(0, 0, 18, background,rolling_background_behaviour,2,1);
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
    entity_t *baddie = baddie_new();

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
