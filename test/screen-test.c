#include "../lib/viewport.h"
#include "../lib/shape.h"
#include "../lib/keys.h"
#include <sys/types.h>
#include <termios.h> // Include termios header


int main() {
    struct termios oldt, newt;

    // Set terminal to non-canonical mode and disable echo
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Create a vp buffer
    struct Viewport vp;
    vp.width = 80;  // Adjust to your desired vp width
    vp.height = 24; // Adjust to your desired vp height

    // Allocate memory for the vp buffer
    viewport__alloc(&vp);

    // Initialize and clear the vp buffer
    viewport__init(&vp);

    // Create a shape and draw it on the vp
    struct Shape shape;
    shape__init(&shape, 10, 5, "Hello");

    
    int count=0;
    while(count<10){
        terminal_clear();
        // Initialize and clear the vp buffer
        viewport__init(&vp);
        shape.x=shape.x+1;
        viewport_shape_draw(&vp, &shape,1,0);

        // Render the vp
        viewport__renderer(&vp);
        count++;
        sleep(1);
    }

    // Deallocate memory for the shape
    shape__dealloc(&shape);

    // Deallocate memory for the vp buffer
    viewport__dealloc(&vp);

    // Restore original terminal settings before exiting
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}
