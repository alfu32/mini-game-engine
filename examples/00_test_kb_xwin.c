#include <stdio.h>
#include <signal.h>
#include "lib/lib.h"


// Declare a flag for signal handling
volatile sig_atomic_t sigint_received = 0;

// Signal handler function
void sigint_handler(int signum) {
    if (signum == SIGINT) {
        sigint_received = 1;
    }
}

#ifdef __linux__
#define USE_WAYLAND 1

#ifdef USE_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

Display *x11_display_ref;
Window root_window;

int initX11() {
    x11_display_ref = XOpenDisplay(NULL);
    if (x11_display_ref) {
        root_window = DefaultRootWindow(x11_display_ref);
        return 1; // X11 is available
    }
    return 0; // X11 is not available
}

void handleX11KeyPress(XKeyEvent *event) {
    KeySym key_symbol = XLookupKeysym(event, 0);
    printf("X11 Key Press: KeySym = %lu, KeyCode = %u\n", key_symbol, event->keycode);
}

void handleX11KeyRelease(XKeyEvent *event) {
    KeySym key_symbol = XLookupKeysym(event, 0);
    printf("X11 Key Release: KeySym = %lu, KeyCode = %u\n", key_symbol, event->keycode);
}

#endif  // USE_X11

#ifdef USE_WAYLAND
#include <wayland-client.h>
#include <wayland-client-protocol.h>

struct wl_display *wayland_display_ref;

// Callback function for global objects
static void registry_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    printf("Registry Global: name=%u, interface=%s, version=%u\n", name, interface, version);

    // You can handle different global objects here based on the interface
    if (strcmp(interface, "wl_compositor") == 0) {
        // Handle wl_compositor global
    } else if (strcmp(interface, "wl_seat") == 0) {
        // Handle wl_seat global
    }
}

// Callback function for global removal
static void registry_global_remove(void *data, struct wl_registry *registry, uint32_t name) {
    printf("Registry Global Removed: name=%u\n", name);

    // You can handle global removal events here
}

int initWayland() {
    wayland_display_ref = wl_display_connect(NULL);
    if (wayland_display_ref) {
        return 1; // Wayland is available
    }
    return 0; // Wayland is not available
}

void handleWaylandKeyPress(struct wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
    printf("Wayland Key Press: Key = %u, State = %u\n", key, state);
}

void handleWaylandKeyRelease(struct wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key) {
    printf("Wayland Key Release: Key = %u\n", key);
}

#endif  // USE_WAYLAND

#endif  // __linux__

int main() {

    // Set up the SIGINT signal handler
    signal(SIGINT, sigint_handler);
    READKEY_INIT
#ifdef __linux__
    int x11Available = 0;
    int waylandAvailable = 0;

    // Check if X11 is available
    #ifdef USE_X11
    x11Available = initX11();
    #endif

    // Check if Wayland is available
    #ifdef USE_WAYLAND
    waylandAvailable = initWayland();
    #endif

    if (x11Available) {
        printf("X11 is available\n");

        // Initialize X11 keyboard
        #ifdef USE_X11
        XSelectInput(x11_display_ref, root_window, KeyPressMask | KeyReleaseMask);
        XEvent event;

        // Rest of your code remains unchanged

        while (!sigint_received) {
            XNextEvent(x11_display_ref, &event);

            if (event.type == KeyPress) {
                handleX11KeyPress(&event.xkey);
            } else if (event.type == KeyRelease) {
                handleX11KeyRelease(&event.xkey);
            }
        }
        #endif
    }
    if (waylandAvailable) {
        printf("Wayland is available\n");

        // Initialize Wayland keyboard
        #ifdef USE_WAYLAND
        struct wl_registry *registry;
        struct wl_compositor *compositor;
        struct wl_seat *seat;
        struct wl_keyboard *keyboard;

        registry = wl_display_get_registry(wayland_display_ref);
        // Initialize Wayland registry and add the listener
        static const struct wl_registry_listener registry_listener = {
            .global = registry_global,
            .global_remove = registry_global_remove,
        };
        wl_registry_add_listener(registry, &registry_listener, NULL);
        wl_display_dispatch(wayland_display_ref);
        wl_display_roundtrip(wayland_display_ref);

        if (!compositor || !seat) {
            fprintf(stderr, "Failed to find Wayland compositor and seat\n");
            return 1;
        }

        keyboard = wl_seat_get_keyboard(seat);
        if (!keyboard) {
            fprintf(stderr, "Failed to get Wayland keyboard\n");
            return 1;
        }
            // Set up the keyboard listener
        static const struct wl_keyboard_listener keyboard_listener = {
            .keymap = keyboard_keymap,
            .enter = keyboard_enter,
            .leave = keyboard_leave,
            .key = keyboard_key,
            .modifiers = keyboard_modifiers,
            .repeat_info = keyboard_repeat_info,
        };

        wl_keyboard_add_listener(keyboard, &keyboard_listener, NULL);
        wl_display_dispatch(wayland_display_ref);
        wl_display_roundtrip(wayland_display_ref);

        while (!sigint_received) {
            wl_display_dispatch(wayland_display_ref);
        }
        #endif
    } else {
        printf("Neither X11 nor Wayland is available\n");
        // Handle the case when both X11 and Wayland are unavailable
    }
#else
    printf("Linux is not detected\n");
#endif
    READKEY_DEINIT
    printf("Exiting...\n");

    return 0;
}
