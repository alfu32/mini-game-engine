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



    printf(" starting \n");
    
    usleep(2000);
    while(running){
        keyboard__refresh(keyboard);
        for(int i=0;i<KEY_MAX;i++){
            if(keyboard->pressed[i])printf("%d,",keyboard->pressed[i]);
        }
        fflush(stdout);
        usleep(2000);
    }
    printf(" - cleaning up \n");
    sleep(1);
    printf(" - resetting terminal \n");
    sleep(1);

    // Clean up and exit
    keyboard__deinit(keyboard);
    sleep(1);
    printf(" - done \n");
    sleep(1);
    return 0;
}

