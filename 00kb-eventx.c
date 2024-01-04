#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "lib/keyboard.h"

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

    while (running) {
        char *pressed_keys = keyboard_get_pressed(keyboard);
        if (pressed_keys != NULL) {
            printf("Pressed keys: ");
            for (int i = 0; pressed_keys[i] != 0; i++) {
                printf("%c ", pressed_keys[i]);
            }
            printf("\n");
            free(pressed_keys);
        }

        usleep(100000); // Sleep for 100ms to avoid high CPU usage
    }

    // Clean up and exit
    keyboard_deinit(keyboard);

    return 0;
}
