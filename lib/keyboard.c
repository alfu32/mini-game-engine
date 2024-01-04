#ifndef KEYBOARD_C
#define KEYBOARD_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libevdev/libevdev.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <linux/input.h>
#include "keyboard.h"

Keyboard* keyboard_new() {
    Keyboard *keyboard = (Keyboard *)malloc(sizeof(Keyboard));
    tcgetattr(STDIN_FILENO, &(keyboard->oldt));
    keyboard->newt = keyboard->oldt;
    keyboard->newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &(keyboard->newt));

    if (keyboard == NULL) {
        fprintf(stderr, "Failed to allocate memory for Keyboard\n");
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

    int num_keyboards = find_keyboard_devices(keyboard->device_paths);

    if (num_keyboards < 0) {
        fprintf(stderr, "Error finding keyboard devices\n");
        free(keyboard->device_paths);
        free(keyboard);
        return NULL;
    }

    keyboard->num_devices = num_keyboards;

    return keyboard;
}

int is_keyboard(const char *device_path) {
    if (strcmp(device_path, "/dev/input/event19") == 0) {
        return 1;
    } else {
        return 0;
    }

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
    /////////// int is_keyboard = libevdev_has_event_type(dev, EV_KEY);
    /////////// 
    /////////// libevdev_free(dev);
    /////////// close(fd);
    /////////// 
    /////////// return is_keyboard;
}

int find_keyboard_devices(char **device_paths) {
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
            char device_path[256];
            snprintf(device_path, sizeof(device_path), "/dev/input/%s", entry->d_name);

                printf("checking keyboard  %s : ", device_path);
            if (is_keyboard(device_path)) {
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
int keyboard_refresh(Keyboard *self) {
    int fd;
    struct input_event ev;

    // Open the input event device (replace '/dev/input/eventX' with your device)
    fd = open("/dev/input/event19", O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("Error opening input device");
        exit(1);
    }
    /////////// int i=10;
    /////////// while (i>0){
        ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
        if (bytesRead == -1) {
            perror("Error reading input event");
    ///////////     break;
        } else if (bytesRead == sizeof(struct input_event)) {
            if (ev.type == EV_KEY) {
                if (ev.value == 0) {
                    printf("Key released: %d\n", ev.code);
                    self->key_state[ev.code] = 0;
                } else if (ev.value == 1) {
                    printf("Key pressed: %d\n", ev.code);
                    self->key_state[ev.code] = ev.value;
                }
                fflush(stdout);
            }
        }
    ///////////     i--;
    ///////////     usleep(10000);
    /////////// }

    // Close the input device
    close(fd);

    return 0;
}

int keyboard_refresh_0(Keyboard *self) {
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

int keyboard_contains(Keyboard *self, const char *keys) {
    keyboard_refresh(self);

    for (int i = 0; keys[i]; i++) {
        int key = keys[i];
        if (self->key_state[key] == 0) {
            fprintf(stderr, "Key %d not found on any keyboard device\n", key);
            return -1;
        }
    }

    return 0;
}

char* keyboard_get_pressed(Keyboard *self) {
    keyboard_refresh(self);
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
            index_copy[i] = key;
            i += 1;
        }
    }

    return index_copy;
}

int keyboard_deinit(Keyboard *self) {
    tcsetattr(STDIN_FILENO, TCSANOW, &(self->oldt));

    for (int i = 0; i < self->num_devices; i++) {
        free(self->device_paths[i]);
    }

    free(self->device_paths);
    free(self);

    return 0;
}

#endif