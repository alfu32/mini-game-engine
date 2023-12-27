#include <stdlib.h>
#include <stdarg.h>


// Define a struct for a shape
struct Shape {
    int x;
    int y;
    char* content;
};

typedef struct Shape Shape;

// Function to allocate memory for a shape
Shape* shape_new(int x, int y, const char* content) {
    Shape* shape=(Shape*)malloc(sizeof(Shape));
    (*shape).x = x;
    (*shape).y = y;
    shape->content = strdup(content);
    return shape;
}
// Function to allocate memory for a shape
void shape_init(Shape* shape, int x, int y, const char* content) {
    (*shape).x = x;
    (*shape).y = y;
    shape->content = strdup(content);
}
void shape_move_to(Shape* shape,int x,int y){
    (*shape).x = x;
    (*shape).y = y;
}
// Function to change the content of a shape
void shape_set(Shape* shape, const char* newContent) {
    free(shape->content);
    shape->content = strdup(newContent);
}
void shape_set_fmt(Shape* shape, const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    // Calculate the length of the formatted string
    int length = vsnprintf(NULL, 0, format, args) + 10; // +1 for the null terminator

    // Allocate memory for the new content
    char* newContent = (char*)malloc(length * sizeof(char));
    if (newContent == NULL) {
        // Handle memory allocation error
        return;
    }

    // Format the content string
    vsnprintf(newContent, length, format, args);

    // Free the old content and assign the new content
    free(shape->content);
    shape->content = newContent;

    va_end(args);
}

// Function to deallocate memory for a shape
void shape_dealloc(Shape* shape) {
    free(shape->content);
    shape->content = NULL;
}