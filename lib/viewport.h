#ifndef SCREEN_H
#define SCREEN_H
    #define BLANK ' '

    // Define a struct for the viewport buffer
    struct Viewport {
        int width;
        int height;
        char** buffer;
    };
    typedef struct Viewport Viewport;

    void viewport_alloc(Viewport* viewport);
    void viewport_init(Viewport* viewport);
    Viewport* viewport_new(int width,int height);
    void viewport_clear(Viewport* viewport);
    void viewport_draw_char(Viewport* viewport, int x, int y, char character);
    void viewport_renderer(const Viewport* viewport);
    void viewport_dealloc(Viewport* viewport);

#endif