
#ifndef _SIMULATOR_C
#define _SIMULATOR_C
#include "include/keyboard.h"
#include "include/viewport.h"
#include "include/scene.h"
#include "include/simulator.h"
#include "include/util.h"
#include <unistd.h>

int run(simulator_t* sim) {
    int frame=0;
    double fps;
    long long duration;
    long long t0=micros();
    long long last_update=t0;
    while(sim->is_running){
        long long d0=micros();

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
            long long t1 = micros();
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

    // these 2 statements are here to keep the warnings out
    // TODO find a better solution
    duration+=1;fps+=1;
    return 0;
}

#endif