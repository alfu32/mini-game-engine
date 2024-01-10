
#include "lib/lib.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>


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
    Viewport* vpp=viewport__new(80,25);

    // Create a shape and draw it on the vpp
    Shape* shape=shape__new(10,5,"HooHoo");
    // Create a shape and draw it on the vpp
    Shape* status=shape__new(0,0,":::GAME:::..........................................");

    
    int count=0;
    unsigned int key = 0x0020;
    while(running){
        terminal_clear();
        if (key_is_pressed()) {
            key = key_read();
            // Handle the key input here
            switch(key){
                case 'z':
                    shape->x-=1;
                    break;
                case 'c':
                    shape->x+=1;
                    break;
                case 's':
                    shape->y-=1;
                    break;
                case 'x':
                    shape->y+=1;
                    break;
                case 'h':
                    shape->y+=1;
                    break;
                case EOF:
                    break;
            }

        }
        /// free(status->content);
        shape__set_content_fmt(status,":::GAME::: [%c] x:%6d y:%6d,frame:%8d ",key, shape->x,shape->y,count);
        // Initialize and clear the vpp buffer
        viewport__clear(vpp);
        viewport_shape_draw(vpp, shape,1,0);
        viewport_shape_draw(vpp, status,2,0);
        // Render the vpp
        viewport__renderer(vpp);
        count++;
        usleep(10000);
    }
    printf(" - cleaning up \n");
    sleep(1);

    // Deallocate memory for the shape
    shape__dealloc(shape);
    shape__dealloc(status);

    // Deallocate memory for the vpp buffer
    viewport__dealloc(vpp);
    printf(" - resetting terminal \n");
    sleep(1);

    READKEY_DEINIT
    result = system("xset r rate 250 50");
    sleep(1);
    printf(" - done \n");
    sleep(1);
    return 0;
}
