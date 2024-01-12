#ifndef KEYBOARD_C
#define KEYBOARD_C
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <dirent.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <termios.h>
    #include <linux/input.h>
    #include <linux/input-event-codes.h>
    #include "include/keyboard.h"

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

    keyboard_t* keyboard__new(const char* device_path) {
        keyboard_t *keyboard = (keyboard_t *)malloc(sizeof(keyboard_t));
        keyboard->device_path=strdup(device_path);
        tcgetattr(STDIN_FILENO, &(keyboard->oldt));
        keyboard->newt = keyboard->oldt;
        keyboard->newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &(keyboard->newt));

        if (keyboard == NULL) {
            fprintf(stderr, "Failed to allocate memory for keyboard_t\n");
            return NULL;
        }

        keyboard->device_paths = (char **)malloc(sizeof(char *) * KEYBOARD_MAX_DEVICES);
        keyboard->num_devices = 0;

        if (keyboard->device_paths == NULL) {
            fprintf(stderr, "Failed to allocate memory for device_paths\n");
            free(keyboard);
            return NULL;
        }

        for (int i = 0; i < KEYBOARD_MAX_DEVICES; i++) {
            keyboard->device_paths[i] = NULL;
        }

        memset(keyboard->key_state, 0, sizeof(keyboard->key_state));

        int num_keyboards = keyboard__find_keyboard_devices(keyboard,keyboard->device_paths);

        if (num_keyboards < 0) {
            fprintf(stderr, "Error finding keyboard devices\n");
            free(keyboard->device_paths);
            free(keyboard);
            return NULL;
        }

        keyboard->num_devices = num_keyboards;



        // Open the input event device (replace '/dev/input/eventX' with your device)
        int fd = open(device_path, O_RDONLY | O_NONBLOCK);
        if (fd == -1) {
            perror("Error opening input device");
            exit(1);
        }
        keyboard->device = fd;

        return keyboard;
    }

    int keyboard__device_is_keyboard(keyboard_t *kb,const char *device_path) {
        return strcmp(device_path, kb->device_path) == 0;

        /////////// struct libevdev *dev = NULL;
        /////////// int fd = open(device_path, O_RDONLY | O_NONBLOCK);
        /////////// 
        /////////// if (fd < 0) {
        ///////////     perror("Failed to open device");
        ///////////     return 0;
        /////////// }
        /////////// 
        /////////// int rc = libevdev_new_from_fd(fd, &dev);
        /////////// 
        /////////// if (rc < 0) {
        ///////////     perror("Failed to initialize libevdev");
        ///////////     close(fd);
        ///////////     return 0;
        /////////// }
        /////////// 
        /////////// int keyboard__device_is_keyboard = libevdev_has_event_type(dev, EV_KEY);
        /////////// 
        /////////// libevdev_free(dev);
        /////////// close(fd);
        /////////// 
        /////////// return keyboard__device_is_keyboard;
    }

    int keyboard__find_keyboard_devices(keyboard_t *kb,char **device_paths) {
        DIR *dir;
        struct dirent *entry;
        int num_keyboards = 0;

        dir = opendir("/dev/input");

        if (dir == NULL) {
            perror("Failed to open /dev/input directory");
            return -1;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (strncmp(entry->d_name, "event", 5) == 0) {
                char device_path[280];
                snprintf(device_path, sizeof(device_path), "/dev/input/%s", entry->d_name);

                    printf("checking keyboard  [%s]/[%s] : ", device_path,kb->device_path);
                // if (keyboard__device_is_keyboard(kb,device_path)) {
                if ( 0 == strcmp(kb->device_path,device_path)) {
                    printf("YES\n");
                    device_paths[num_keyboards] = strdup(device_path);
                    num_keyboards++;
                } else {
                    printf("no\n");
                }
            }
        }

        closedir(dir);
        return num_keyboards;
    }
    
    int keyboard__refresh(keyboard_t *self) {
        ssize_t bytesRead = read(self->device, &(self->ev), sizeof(struct input_event));
        if (bytesRead == -1) {
            // perror("Error reading input event");
        } else if (bytesRead == sizeof(struct input_event)) {
            if (self->ev.type == EV_KEY) {
                if (self->ev.value == 0) {
                    //printf("\rKey released: %d\n", map_keycode_to_char(self->ev.code));
                    self->key_state[self->ev.code] = 0;
                } else if (self->ev.value == 1) {
                    //printf("\rKey pressed: %d\n", map_keycode_to_char(self->ev.code));
                    self->key_state[self->ev.code] = map_keycode_to_char(self->ev.code);
                }
                //fflush(stdout);
            }
        }

        return 0;
    }

    int keyboard_refresh_0(keyboard_t *self) {
        for (int i = 0; i < self->num_devices; i++) {
            int fd = open(self->device_paths[i], O_RDONLY | O_NONBLOCK);

            if (fd < 0) {
                perror("Failed to open device");
                return -1;
            }

            struct input_event ev;
            ssize_t bytes_read;

            while ((bytes_read = read(fd, &ev, sizeof(ev))) > 0) {
                if (ev.type == EV_KEY && ev.code < KEY_MAX) {
                    self->key_state[ev.code] = ev.value;
                }
            }

            close(fd);
        }
        return 0;
    }

    int keyboard__contains_keys(keyboard_t *self, const char *keys) {
        keyboard__refresh(self);

        for (int i = 0; keys[i]; i++) {
            int key = keys[i];
            if (self->key_state[key] == 0) {
                fprintf(stderr, "Key %d not found on any keyboard device\n", key);
                return -1;
            }
        }

        return 0;
    }


    char* keyboard__fetch_pressed(keyboard_t *self) {
        keyboard__refresh(self);
        int num_keys = 0;

        for (int key = 0; key < KEY_MAX; key++) {
            if (self->key_state[key] != 0) {
                num_keys += 1;
            }
        }

        if (num_keys == 0) {
            return NULL;
        }

        char *index_copy = (char *)malloc(sizeof(char) * num_keys);
        int i = 0;

        for (int key = 0; key < KEY_MAX; key++) {
            if (self->key_state[key] != 0) {
                index_copy[i] = self->key_state[key];
                i += 1;
            }
        }

        return index_copy;
    }

    int keyboard__deinit(keyboard_t *self) {
        tcsetattr(STDIN_FILENO, TCSANOW, &(self->oldt));

        // Close the input device
        close(self->device);

        for (int i = 0; i < self->num_devices; i++) {
            free(self->device_paths[i]);
        }

        free(self->device_paths);
        free(self->device_path);
        free(self);

        return 0;
    }

#endif