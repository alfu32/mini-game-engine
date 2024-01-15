#ifndef _SIMULATOR_H
#define _SIMULATOR_H
#include <unistd.h>
#include "keyboard.h"
#include "viewport.h"
#include "scene.h"
#include "draw.h"
#include "util.h"

typedef struct simulator_t simulator_t;

typedef void (*simulator_void_callback_fn)(simulator_t* sim,int frame);

typedef struct simulator_t{
    keyboard_t *keyboard;
    viewport_t* viewport;
    scene_manager_t* manager;
    int is_running;
    simulator_void_callback_fn on_before_frame;
    simulator_void_callback_fn on_after_frame;
    int max_frame_time_micros;
}simulator_t;

int run(simulator_t* sim);

#endif