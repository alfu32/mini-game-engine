#ifndef SHAPE_C
#define SHAPE_C
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include <string.h>
    #include "rect.h"
    #include "shape.h"

    // Function to allocate memory for a shape
    Shape* shape__new(int x, int y, const char* content) {
        Shape* shape=(Shape*)malloc(sizeof(Shape));
        (*shape).x = x;
        (*shape).y = y;
        (*shape).z = 0;
        shape->content = strdup(content);
        return shape;
    }

    // Function to copy a shape
    void shape__copy(Shape* this, Shape source) {
        if (this->content) {
            free(this->content); // Free the old content
        }
        this->x = source.x;
        this->y = source.y;
        this->content = strdup(source.content); // Allocate and copy new content
    }

    // Function to allocate memory for a shape
    void shape__init(Shape* shape, int x, int y, const char* content) {
        (*shape).x = x;
        (*shape).y = y;
        shape->content = strdup(content);
    }
    void shape__move_to(Shape* shape,int x,int y){
        (*shape).x = x;
        (*shape).y = y;
    }
    // Function to change the content of a shape
    void shape__set(Shape* shape, const char* newContent) {
        free(shape->content);
        shape->content = strdup(newContent);
    }
    void shape__set_fmt0(Shape* shape, const char* format, ...) {
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
    void shape__set_content_fmt(Shape* shape, const char* format, ...) {
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


    size_t countLines(const char* string) {
        if (string == NULL) {
            return 0;
        }

        size_t lineCount = 0;
        char isNewLine = 1; // Flag to track if we're at the beginning of a new line

        while (*string != '\0') {
            if (*string == '\n') {
                lineCount++;
                isNewLine = 1; // Set the flag for a new line
            } else {
                if (isNewLine) {
                    isNewLine = 0; // Clear the flag for the rest of the line
                }
            }
            string++;
        }

        // Add one for the last line if it doesn't end with a newline
        if (!isNewLine) {
            lineCount++;
        }

        return lineCount;
    }
    // Function to calculate the maximum line width in a string
    size_t findMaxLineWidth(const char* string) {
        if (string == NULL) {
            return 0;
        }

        size_t maxWidth = 0;
        size_t currentWidth = 0;

        while (*string != '\0') {
            if (*string == '\n') {
                if (currentWidth > maxWidth) {
                    maxWidth = currentWidth;
                }
                currentWidth = 0; // Reset the current line width
            } else {
                currentWidth++;
            }
            string++;
        }

        // Check the width of the last line
        if (currentWidth > maxWidth) {
            maxWidth = currentWidth;
        }

        return maxWidth;
    }

    ClientRect* shape__get_bounding_client_rect(Shape* shape) {
        ClientRect *r = (ClientRect*)malloc(sizeof(ClientRect));
        if (shape == NULL) {
            r->x=0;
            r->y=0;
            r->z=-100;
            r->width=-1;
            r->height=-1;
            return r;
        }
        int shapeWidth=findMaxLineWidth(shape->content);
        int shapeHeight=countLines(shape->content);

        struct ClientRect rect={.x=shape->x,.y=shape->y,.z=shape->z,.width=shapeWidth,.height=shapeHeight};
            r->x=shape->x;
            r->y=shape->y;
            r->z=shape->z;
            r->width=shapeWidth;
            r->height=shapeHeight;
        return r;
    }

    // Function to deallocate memory for a shape
    void shape__dealloc(Shape* shape) {
        free(shape->content);
        shape->content = NULL;
    }
#endif