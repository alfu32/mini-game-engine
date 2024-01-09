#ifndef KEYBOARD_H
#define KEYBOARD_H
    #include <termios.h>
    #include <linux/input.h>

    #define KEYBOARD_MAX_DEVICES 128
    /// dchar* KEYBOARD_NOKEYS="---\0";

    typedef struct Keyboard {
        int num_devices;
        struct termios oldt;
        struct termios newt;
        char **device_paths;
        // maps key_code to corresponding char
        // if a key is not pressed the value of the corresponding key_code is 0
        // if a key is pressed the value of the corresponding key_code is the corresponding char
        char key_state[KEY_MAX];
        // fd of the keyboard device
        int device;
        struct input_event ev;
        // TODO remove
        char* device_path;
    } Keyboard;

    Keyboard* keyboard_new();
    int is_keyboard(Keyboard *kb,const char *device_path);
    int find_keyboard_devices(Keyboard *kb,char **device_paths);
    int keyboard_refresh(Keyboard *self);
    int keyboard_contains(Keyboard *self, const char *keys);
    // return NULL when no key is pressed
    char* keyboard_get_pressed(Keyboard *self);
    int keyboard_deinit(Keyboard *self);

#endif