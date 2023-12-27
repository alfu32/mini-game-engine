#include "screen.c"
#include "shape.c"


int main() {
    struct termios oldt, newt;

    // Set terminal to non-canonical mode and disable echo
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Create a screen buffer
    struct Screen screen;
    screen.width = 80;  // Adjust to your desired screen width
    screen.height = 24; // Adjust to your desired screen height

    // Allocate memory for the screen buffer
    screen_alloc(&screen);

    // Initialize and clear the screen buffer
    screen_init(&screen);

    // Create a shape and draw it on the screen
    struct Shape shape;
    shape_init(&shape, 10, 5, "Hello");

    
    int count=0;
    while(count<10){
        terminal_clear();
        // Initialize and clear the screen buffer
        screen_init(&screen);
        shape.x=shape.x+1;
        screen_shape_draw(&screen, &shape);

        // Render the screen
        screen_renderer(&screen);
        count++;
        sleep(1);
    }

    // Deallocate memory for the shape
    shape_dealloc(&shape);

    // Deallocate memory for the screen buffer
    screen_dealloc(&screen);

    // Restore original terminal settings before exiting
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}
