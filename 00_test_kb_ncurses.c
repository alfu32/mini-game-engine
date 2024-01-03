#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include "lib/lib.h"
#define BLANKCHAR '+'

char map_keycode_to_char(int keycode) {
    switch (keycode) {
        case KEY_A: return 'a';
        case KEY_B: return 'b';
        case KEY_C: return 'c';
        case KEY_D: return 'd';
        case KEY_E: return 'e';
        case KEY_F: return 'f';
        case KEY_G: return 'g';
        case KEY_H: return 'h';
        case KEY_I: return 'i';
        case KEY_J: return 'j';
        case KEY_K: return 'k';
        case KEY_L: return 'l';
        case KEY_M: return 'm';
        case KEY_N: return 'n';
        case KEY_O: return 'o';
        case KEY_P: return 'p';
        case KEY_Q: return 'q';
        case KEY_R: return 'r';
        case KEY_S: return 's';
        case KEY_T: return 't';
        case KEY_U: return 'u';
        case KEY_V: return 'v';
        case KEY_W: return 'w';
        case KEY_X: return 'x';
        case KEY_Y: return 'y';
        case KEY_Z: return 'z';
        
        case KEY_1: return '1';
        case KEY_2: return '2';
        case KEY_3: return '3';
        case KEY_4: return '4';
        case KEY_5: return '5';
        case KEY_6: return '6';
        case KEY_7: return '7';
        case KEY_8: return '8';
        case KEY_9: return '9';
        case KEY_0: return '0';
        
        case KEY_MINUS: return '-';
        case KEY_EQUAL: return '=';
        case KEY_LEFTBRACE: return '[';
        case KEY_RIGHTBRACE: return ']';
        case KEY_SEMICOLON: return ';';
        case KEY_APOSTROPHE: return '\'';
        case KEY_GRAVE: return '`';
        case KEY_BACKSLASH: return '\\';
        case KEY_COMMA: return ',';
        case KEY_DOT: return '.';
        case KEY_SLASH: return '/';
        case KEY_SPACE: return ' ';
        
        // Add more cases for other keycodes as needed
        
        default: return BLANKCHAR; // Return null character for unknown keycodes
    }
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

    int fd;
    struct input_event ev;
    char keys[256]={' '};
    for(int i=0;i<256;i++)keys[i]=BLANKCHAR;

    // Open the input event device (replace '/dev/input/eventX' with your device)
    fd = open("/dev/input/event19", O_RDONLY);
    if (fd == -1) {
        perror("Error opening input device");
        exit(1);
    }
    printf("start \n");

    while (running) {
        ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
        if (bytesRead == -1) {
            perror("Error reading input event");
            break;
        } else if (bytesRead == sizeof(struct input_event)) {
            if (ev.type == EV_KEY) {
                if (ev.value == 0) {
                    keys[ev.code]=BLANKCHAR;
                    //printf("Key released: %d\n", ev.code);
                } else if (ev.value == 1) {
                    keys[ev.code]=map_keycode_to_char(ev.code);
                    //printf("Key pressed: %d\n", ev.code);
                }
                terminal_clear();
                printf("%s\r",keys);
                fflush(stdout);
            }
        }
        usleep(10000);
    }

    // Close the input device
    close(fd);

    READKEY_DEINIT

    printf("done \n");

    return 0;
}
