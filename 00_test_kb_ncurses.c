#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

// Define a structure to store key events
struct KeyEvent {
    int key;
    struct KeyEvent* next;
};

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    char pressedKeysStr[256] = {0}; // Initialize the string for pressed keys
    char releasedKeysStr[256] = {0}; // Initialize the string for released keys
    int pressedKeysCount = 0;
    int releasedKeysCount = 0;
    int ch;

    while (1) {
        ch = getch();
        if (ch != ERR) {
            if (ch == KEY_F(1)) {
                // Example: Detecting the F1 key
                mvprintw(1, 5, "F1 key pressed");
            } else if (ch == 27) {
                // Example: Detecting the Escape key (ASCII 27)
                mvprintw(2, 5, "Escape key pressed");
            } else {
                // Store the key value in the pressed keys string
                    pressedKeysStr[ch] = (char)ch;
            }
            refresh();
        } else {
            // Check for key releases and move them from the pressed string to the released string
            for (int i = 0; i < pressedKeysCount; i++) {
                if (!getch()) {
                    // Key released; move it to the released keys string
                    releasedKeysStr[ch] = (char)ch;
                    pressedKeysStr[ch] = ' ';
                    // Shift remaining keys to fill the gap
                    for (int j = i; j < pressedKeysCount - 1; j++) {
                        pressedKeysStr[j] = pressedKeysStr[j + 1];
                    }
                    pressedKeysStr[pressedKeysCount - 1] = '\0';
                    pressedKeysCount--;
                }
            }
        }

        // Print the recorded pressed and released key strings in fixed positions
        clear(); // Clear the screen
        mvprintw(1, 5, "Pressed keys: %s", pressedKeysStr);
        mvprintw(3, 5, "Released keys: %s", releasedKeysStr);
        refresh();
        // Your other program logic here
        usleep(10000);
    }

    endwin();

    return 0;
}
