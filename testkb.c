#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

int main() {
    int fd;
    struct input_event ev;

    // Open the input event device (replace '/dev/input/eventX' with your device)
    fd = open("/dev/input/event19", O_RDONLY);
    if (fd == -1) {
        perror("Error opening input device");
        exit(1);
    }

    while (1) {
        ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
        if (bytesRead == -1) {
            perror("Error reading input event");
            break;
        } else if (bytesRead == sizeof(struct input_event)) {
            if (ev.type == EV_KEY) {
                if (ev.value == 0) {
                    printf("Key released: %d\n", ev.code);
                } else if (ev.value == 1) {
                    printf("Key pressed: %d\n", ev.code);
                }
                fflush(stdout);
            }
        }
    }

    // Close the input device
    close(fd);

    return 0;
}