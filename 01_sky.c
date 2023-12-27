#include "lib/draw.c"
#include "lib/keys.c"

int main() {

    READKEY_INIT

    // Create a screen buffer
    Screen* screen=screen_new(80,25);

    // Create a shape and draw it on the screen
    Shape* shape=shape_new(10,5,"HooHoo");
    // Create a shape and draw it on the screen
    Shape* status=shape_new(0,0,":::GAME:::..........................................");

    
    int count=0;
    unsigned int key = 0x0020;
    while(1){
        terminal_clear();
        if (key_is_pressed()) {
            key = key_read();
            // Handle the key input here
            switch(key){
                case 'z':
                    shape->x-=1;
                    break;
                case 'c':
                    shape->x+=1;
                    break;
                case 's':
                    shape->y-=1;
                    break;
                case 'x':
                    shape->y+=1;
                    break;
                case EOF:
                    break;
            }

        }
        /// free(status->content);
        snprintf(status->content,80,":::GAME::: [%c] x:%6d y:%6d,frame:%8d ",key, shape->x,shape->y,count);
        // Initialize and clear the screen buffer
        screen_clear(screen);
        screen_shape_draw(screen, shape);
        screen_shape_draw(screen, status);
        // Render the screen
        screen_renderer(screen);
        count++;
        usleep(10000);
    }

    // Deallocate memory for the shape
    shape_dealloc(shape);
    shape_dealloc(status);

    // Deallocate memory for the screen buffer
    screen_dealloc(screen);

    READKEY_DEINIT

    return 0;
}
