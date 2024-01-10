#ifndef SCREEN_H
#define SCREEN_H
    #define BLANK ' '
    #define COLOR_RESET "\x1b[0m"

    #define COLOR_BLACK "\x1b[30m"
    #define COLOR_RED "\x1b[31m"
    #define COLOR_GREEN "\x1b[32m"
    #define COLOR_YELLOW "\x1b[33m"
    #define COLOR_BLUE "\x1b[34m"
    #define COLOR_MAGENTA "\x1b[35m"
    #define COLOR_CYAN "\x1b[36m"
    #define COLOR_WHITE "\x1b[37m"

    #define COLOR_BRIGHT_BLACK "\x1b[90m"
    #define COLOR_BRIGHT_RED "\x1b[91m"
    #define COLOR_BRIGHT_GREEN "\x1b[92m"
    #define COLOR_BRIGHT_YELLOW "\x1b[93m"
    #define COLOR_BRIGHT_BLUE "\x1b[94m"
    #define COLOR_BRIGHT_MAGENTA "\x1b[95m"
    #define COLOR_BRIGHT_CYAN "\x1b[96m"
    #define COLOR_BRIGHT_WHITE "\x1b[97m"

    #define BACKGROUND_COLOR_BLACK "\x1b[40m"
    #define BACKGROUND_COLOR_RED "\x1b[41m"
    #define BACKGROUND_COLOR_GREEN "\x1b[42m"
    #define BACKGROUND_COLOR_YELLOW "\x1b[43m"
    #define BACKGROUND_COLOR_BLUE "\x1b[44m"
    #define BACKGROUND_COLOR_MAGENTA "\x1b[45m"
    #define BACKGROUND_COLOR_CYAN "\x1b[46m"
    #define BACKGROUND_COLOR_WHITE "\x1b[47m"

    #define BACKGROUND_BRIGHT_COLOR_BLACK "\x1b[100m"
    #define BACKGROUND_BRIGHT_COLOR_RED "\x1b[101m"
    #define BACKGROUND_BRIGHT_COLOR_GREEN "\x1b[102m"
    #define BACKGROUND_BRIGHT_COLOR_YELLOW "\x1b[103m"
    #define BACKGROUND_BRIGHT_COLOR_BLUE "\x1b[104m"
    #define BACKGROUND_BRIGHT_COLOR_MAGENTA "\x1b[105m"
    #define BACKGROUND_BRIGHT_COLOR_CYAN "\x1b[106m"
    #define BACKGROUND_BRIGHT_COLOR_WHITE "\x1b[107m"

    // Define a struct for the viewport buffer
    struct viewport_t {
        int width;
        int height;
        char** buffer;
        char** colors;
        char** backgrounds;
    };
    typedef struct viewport_t viewport_t;

    void viewport__alloc(viewport_t* viewport);
    void viewport__init(viewport_t* viewport);
    viewport_t* viewport__new(int width,int height);
    void viewport__clear(viewport_t* viewport);
    void viewport__draw_char(viewport_t* viewport, int x, int y, char character,char fg,char bg);
    void viewport__renderer(const viewport_t* viewport);
    void viewport__dealloc(viewport_t* viewport);

#endif