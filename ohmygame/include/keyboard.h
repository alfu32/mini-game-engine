#ifndef KEYBOARD_H
#define KEYBOARD_H
    #include <termios.h>
    #include <linux/input.h>

    #define KEYBOARD_MAX_DEVICES 128
    /// dchar* KEYBOARD_NOKEYS="---\0";

    typedef struct keyboard_t {
        int num_devices;
        struct termios oldt;
        struct termios newt;
        char **device_paths;
        // maps key_code to corresponding char
        // if a key is not pressed the value of the corresponding key_code is 0
        // if a key is pressed the value of the corresponding key_code is the corresponding char
        char key_state[KEY_MAX];
        int pressed[KEY_MAX];
        // fd of the keyboard device
        int device;
        struct input_event ev;
        // TODO remove
        char* device_path;
    } keyboard_t;

    keyboard_t* keyboard__new(const char* device_path);
    int keyboard__device_is_keyboard(keyboard_t *kb,const char *device_path);
    int keyboard__find_keyboard_devices(keyboard_t *kb,char **device_paths);
    int keyboard__refresh(keyboard_t *self);
    int keyboard__contains_keys(keyboard_t *self, const char *keys);
    int keyboard__contains_key_code(keyboard_t *self, const int code);
    // return NULL when no key is pressed
    char* keyboard__fetch_pressed(keyboard_t *self);
    int keyboard__deinit(keyboard_t *self);

#endif