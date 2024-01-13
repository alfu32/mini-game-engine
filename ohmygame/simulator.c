
#ifndef _SIMULATOR_C
#define _SIMULATOR_C
#include "include/keyboard.h"
#include "include/viewport.h"
#include "include/scene.h"

typedef void (*simulator_void_callback_fn)(Simulator* sim,int frame);

typedef struct Simulator{
    keyboard_t *keyboard;
    viewport_t* viewport;
    scene_manager_t* manager;
    int is_running;
    simulator_void_callback_fn on_before_frame;
    simulator_void_callback_fn on_after_frame;
    int max_frame_time_micros;
}Simulator;

int run(Simulator* sim) {
    int frame=0;
    double fps;
    long long duration;
    long long t0=nanos()/1000;
    long long last_update=t0;
    while(sim->is_running){
        long long d0=nanos()/1000;

        char *pressed = keyboard__fetch_pressed(sim->keyboard);
        if(0 || d0-last_update > sim->max_frame_time_micros){
            terminal_clear();

            sim->on_before_frame(sim,frame);

            viewport__clear(sim->viewport);
            scene_manager__update(sim->manager, frame,pressed);
            /// viewport_shape_draw(sim->viewport, status,3,0);
            scene_manager__draw_on_viewport(sim->manager,sim->viewport);
            // Render the vpp
            viewport__renderer(sim->viewport);
            scene_manager__do_collisions(sim->manager);
            scene_manager__remove_dead_shapes(sim->manager);
            last_update=d0;
            long long t1 = nanos()/1000;
            duration=t1-d0;
            fps=frame/(t1-t0);

            sim->on_after_frame(sim,frame);

            frame++;
        }
        usleep(1000);

        if(pressed != NULL){
            free(pressed);
        }
    }
    return 0;
}
#endif