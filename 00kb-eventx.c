#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "lib/keyboard.h"
#include "lib/keys.h"

static volatile int running = 1;

void sigint_handler(int signum) {
    running = 0;
}

int main() {
    // Create a Keyboard object
    Keyboard *keyboard = keyboard_new();
    if (keyboard == NULL) {
        fprintf(stderr, "Failed to initialize Keyboard\n");
        return 1;
    }

    // Set up a SIGINT handler to exit gracefully
    signal(SIGINT, sigint_handler);

    printf("Press Ctrl+C to exit.\n");
    unsigned long long frame=0;
    const char * roll="-\\|/";
    
    while (running) {
            char *pressed_keys = keyboard_get_pressed(keyboard);
            /// if(pressed_keys != NULL){
            ///     for (int i = 0; pressed_keys[i] != 0; i++) {
            ///         putchar(pressed_keys[i]);
            ///     }
            /// }

            if(pressed_keys != NULL){
                printf("\r%cPressed keys: %s            ",roll[frame%4],pressed_keys);
                free(pressed_keys);
                usleep(10000);
            } else {
                printf("\r%cPressed keys: ",roll[frame%4]);
                usleep(100);
            }
            frame++;
    }

    // Clean up and exit
    keyboard_deinit(keyboard);

    return 0;
}
