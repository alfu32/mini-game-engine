#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wayland-client.h>
#include <wayland-client-protocol.h>

#define MAX_KEYS 256

struct wl_display *display;
struct wl_registry *registry;
struct wl_seat *seat;
struct wl_keyboard *keyboard;

// Array to store key states (0: released, 1: pressed)
int key_states[MAX_KEYS];

// Wayland callbacks for key press and release
static void keyboard_keymap(void *data, struct wl_keyboard *keyboard, uint32_t format, int32_t fd, uint32_t size) {
    // Handle keymap
}

static void keyboard_enter(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface, struct wl_array *keys) {
    // Handle keyboard focus enter
}

static void keyboard_leave(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface) {
    // Handle keyboard focus leave
}

static void keyboard_key(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
    if (key < MAX_KEYS) {
        key_states[key] = state;
        printf("Key %u %s\n", key, (state == WL_KEYBOARD_KEY_STATE_PRESSED) ? "pressed" : "released");
    }
}

static void keyboard_modifiers(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
    // Handle modifier keys
}

static void keyboard_repeat_info(void *data, struct wl_keyboard *keyboard, int32_t rate, int32_t delay) {
    // Handle keyboard repeat info
}

// Signal handler for Ctrl+C (SIGINT)
volatile int stopping = 0;

void sigint_handler(int signum) {
    if (signum == SIGINT) {
        printf("received sigint\n");
        stopping = 1;
    }
}

// Registry listener to handle Wayland globals
static void registry_global(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version) {
    printf("Registry Global: name=%u, interface=%s, version=%u\n", id, interface, version);
    if (strcmp(interface, "wl_seat") == 0) {
        printf("Initialize the seat and keyboard interfaces\n");
        // Initialize the seat and keyboard interfaces
        seat = wl_registry_bind(registry, id, &wl_seat_interface, 1);
        keyboard = wl_seat_get_keyboard(seat);

        printf("Set up the keyboard listener\n");
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
    }
}

static void registry_global_remove(void *data, struct wl_registry *registry, uint32_t id) {
    printf("Registry Global Removed: name=%u\n", id);
    // Handle global removal if needed
}

int main() {
    // Set up the SIGINT signal handler
    signal(SIGINT, sigint_handler);

    // Initialize Wayland display
    display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to Wayland display\n");
        return 1;
    }

    // Initialize Wayland registry
    registry = wl_display_get_registry(display);

        // Initialize Wayland registry and add the listener
        static const struct wl_registry_listener registry_listener = {
            .global = registry_global,
            .global_remove = registry_global_remove,
        };
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_dispatch(display);
    wl_display_roundtrip(display);

    while (stopping == 0) {
        // Continue running until SIGINT is received
        if(stopping == 1){
            printf("wut?");
        }

        // Handle key states in the key_states array
        for (uint32_t key = 0; key < MAX_KEYS; key++) {
            if (key_states[key] == WL_KEYBOARD_KEY_STATE_PRESSED) {
                // Key pressed, handle it and update the array if needed
                // For example, update the array cell for key 'key'
            } else if (key_states[key] == WL_KEYBOARD_KEY_STATE_RELEASED) {
                // Key released, handle it and update the array if needed
                // For example, clear the array cell for key 'key'
            }
        }

        // Dispatch Wayland events
        wl_display_dispatch(display);
    }

    printf("SIGINT received. Exiting...\n");

    // Clean up and exit gracefully
    if (keyboard) {
        wl_keyboard_release(keyboard);
    }

    if (seat) {
        wl_seat_release(seat);
    }

    wl_registry_destroy(registry);
    wl_display_disconnect(display);

    return 0;
}
