#include "keys.h"
#include <sys/types.h>
#include <stdio.h>

int main() {
    READKEY_INIT

    while (1) {
        if (key_is_pressed()) {
            int key = key_read();

            // Handle the key input here
            if (key != EOF) {
                printf("Key pressed: %c\n", key);
            }
        }

        // Your game logic continues here
    }

    READKEY_DEINIT

    return 0;
}
