#ifndef SHAPE_C
#define SHAPE_C
    #include "shape.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include <string.h>

    // Function to allocate memory for a shape
    Shape* shape_new(int x, int y, const char* content) {
        Shape* shape=(Shape*)malloc(sizeof(Shape));
        (*shape).x = x;
        (*shape).y = y;
        (*shape).z = 0;
        shape->content = strdup(content);
        return shape;
    }

    // Function to copy a shape
    void shape_copy(Shape* this, Shape source) {
        if (this->content) {
            free(this->content); // Free the old content
        }
        this->x = source.x;
        this->y = source.y;
        this->content = strdup(source.content); // Allocate and copy new content
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
    void shape_set_fmt0(Shape* shape, const char* format, ...) {
        va_list args;
        va_start(args, format);
        
        // Calculate the length of the formatted string
        int length = vsnprintf(NULL, 0, format, args) + 1; // +1 for the null terminator

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
    void shape_set_fmt(Shape* shape, const char* format, ...) {
        va_list args;
        va_start(args, format);

        // Allocate memory for the new content
        char* newContent = (char*)malloc(2048 * sizeof(char));
        if (newContent == NULL) {
            // Handle memory allocation error
            return;
        }

        // Format the content string
        vsnprintf(newContent, 2048, format, args);

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
#endif