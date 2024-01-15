#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <linux/input.h>

#define FILE_EVENT_OUTPUT_STREAM "/tmp/kbev/events"

volatile sig_atomic_t service_running = 1;

// Function to handle SIGINT (Ctrl+C)
void interrupt_handler(int signum) {
    printf("Received Ctrl+C. Cleaning up and exiting...\n");
    service_running = 0;
}

// Function to handle SIGHUP
void hup_handler(int signum) {
    printf("Received SIGHUP. Reloading configuration...\n");
    // Implement configuration reloading logic here
}

// Function to handle SIGTERM
void term_handler(int signum) {
    printf("Received SIGTERM. Cleaning up and exiting...\n");
    service_running = 0;
}

// Function to handle SIGQUIT
void quit_handler(int signum) {
    printf("Received SIGQUIT. Cleaning up and exiting...\n");
    service_running = 0;
}

// Function to handle SIGUSR1
void usr1_handler(int signum) {
    printf("Received SIGUSR1. rescan devices ( not implemented yet)\n");
    // Implement custom action 1 logic here
}

// Function to handle SIGUSR2
void usr2_handler(int signum) {
    printf("Received SIGUSR2. Custom action 2...\n");
    // Implement custom action 2 logic here
}


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
            
            // Add more cases for other keycodes as needed
            
            default: return keycode?(char)keycode&0x7F:'\xFF'; // Return null character for unknown keycodes
        }
    }


// Function to aggregate key events and return key_states
int fetch_keyboard_events(char* key_states,const char* device_path,unsigned int wait_ms) {
    struct input_event ev;
    int input_fd = open(device_path, O_RDONLY);
    if (input_fd == -1) {
        perror("Error opening keyboard device");
        close(input_fd);
        return -1;
    }
    ssize_t bytes_read = read(input_fd, &ev, sizeof(ev));
    if (bytes_read == sizeof(ev)/* && ev.type == EV_KEY*/) {
        // Check if the key is pressed (value is 1) and output it
        if(ev.code > KEY_MAX) {
            printf("event code %d is out of range\n",ev.code);
            close(input_fd);
            return -2;
        }
        if (ev.value == 1) {
            key_states[ev.code] = 1; // Update key state
        } else {
            key_states[ev.code] = 0; // Update key state
        }
    }
    close(input_fd);
    return 0;
}

// Function to aggregate key events and return key_states
int fetch_keyboard_events_async(unsigned char* key_states,const char* device_path,unsigned int wait_ms) {
    struct input_event ev;
    int input_fd = open(device_path, O_RDONLY | O_NONBLOCK);
    if (input_fd == -1) {
        perror("Error opening keyboard device");
        close(input_fd);
        return -1;
    }
    for(int i=0;i<wait_ms;i++){
        ssize_t bytes_read = read(input_fd, &ev, sizeof(ev));
        if (bytes_read == sizeof(ev)/* && ev.type == EV_KEY*/) {
            // Check if the key is pressed (value is 1) and output it
            if(ev.code > KEY_MAX) {
                printf("event code %d is out of range\n",ev.code);
                close(input_fd);
                return -2;
            }
            if (ev.value == 1) {
                key_states[ev.code] = ev.value; // Update key state
            } else {
                key_states[ev.code] = 0; // Update key state
            }
        }
        usleep(1000);
    }
    close(input_fd);
    return 0;
}


void disableTerminalOutput() {
    struct termios term;
    
    // Get the current terminal settings
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    
    // Disable echoing and canonical mode
    term.c_lflag &= ~(ECHO | ICANON);
    
    // Set the modified terminal settings
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

void enableTerminalOutput() {
    struct termios term;
    
    // Get the current terminal settings
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    
    // Restore original terminal settings
    term.c_lflag |= (ECHO | ICANON);
    
    // Set the modified terminal settings
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

int main(int devices_count,const char **device_files) {
    #ifdef DEBUG
        printf("running in debug mode \n");
    #endif

    if(devices_count < 2 ) {
        printf("please specify at least one input device file\n");
        return -1;
    }
    #ifdef DEBUG
        printf("\ngot %d arguments\n",devices_count);
    #endif
    // Set signal handlers
    signal(SIGINT, interrupt_handler);
    signal(SIGHUP, hup_handler);
    signal(SIGTERM, term_handler);
    signal(SIGQUIT, quit_handler);
    signal(SIGUSR1, usr1_handler);
    signal(SIGUSR2, usr2_handler);

    char* key_states=(char*)malloc(sizeof(char)*(KEY_MAX + 1) );
    disableTerminalOutput();
    int return_value=0;
    while (service_running) {
        /// int device_result=fetch_keyboard_events_async(key_states, device_files[1],120);
        for(int device_index=1;device_index<devices_count;device_index++){
            const char* device_file=device_files[device_index];
            int device_result=fetch_keyboard_events_async(key_states, device_file,120);
            if(device_result != 0){
                printf("Warning: polling device %s returned error %d",device_file,device_result);
            }
        }


        // Open the eventX file for writing
        int eventX_fd;
        ////// waiting for a handle to output file
        /// while( (service_running != 0) && (eventX_fd = open(FILE_EVENT_OUTPUT_STREAM, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR, 0644))!=0 );
        eventX_fd = open(FILE_EVENT_OUTPUT_STREAM, O_WRONLY | O_CREAT,0777);
        // Loop through the keymap and write non-zero characters to the file
        for (int i = 0; i < KEY_MAX; i++) {
            if (key_states[i] != 0) {
                if (write(eventX_fd, &key_states[i], 1) == -1) {
                    perror("Error writing to file output stream");
                    return_value=1;
                    service_running=0;
                }
            }
        }

        printf("\rsome keys were pressed : [");
        for (int code = 0; code < KEY_MAX; code++) {
            if (key_states[code] != 0) {
                printf("[%d](%c),",code,map_keycode_to_char(code));
            }
        }
        printf("]          ");
        fflush(stdout);
        // Close the file
        close(eventX_fd);
    }
    enableTerminalOutput();
    free(key_states);

    printf("Keyboard event service exited\n");

    return return_value;
}