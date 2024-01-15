#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disable_echo() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;
    new_termios.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void reset_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

int main() {
    Display *display;
    Window root;
    XEvent ev;

    // Disable echo in the terminal
    disable_echo();

    // Open the X display
    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Failed to open X display\n");
        return 1;
    }

    root = DefaultRootWindow(display);

    // Select key press and key release events
    XSelectInput(display, root, KeyPressMask | KeyReleaseMask);

    printf("Press keys (Ctrl+C to exit):\n");

    while (1) {
        XNextEvent(display, &ev);

        if (ev.type == KeyPress) {
            KeySym keysym = XLookupKeysym(&ev.xkey, 0);
            printf("Key press event: %c\n", (char)keysym);
        } else if (ev.type == KeyRelease) {
            KeySym keysym = XLookupKeysym(&ev.xkey, 0);
            printf("Key release event: %c\n", (char)keysym);
        } else {
            KeySym keysym = XLookupKeysym(&ev.xkey, 0);
            printf("Key OTHER event: %c, evtype: %d\n", (char)keysym, ev.type);
        }
    }

    // Reset terminal to initial values before exiting
    reset_terminal();

    XCloseDisplay(display);
    return 0;
}
