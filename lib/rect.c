#ifndef RECT_C
#define RECT_C
    #include "rect.h"
    #include "shape.h"
    #include <string.h>
    #include <stddef.h>

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

    ClientRect get_bounding_client_rect(Shape* shape) {
        if (shape == NULL) {
            ClientRect cr={.x=0,.y=0,.width=0,.height=0};
            return cr;
        }
        int shapeWidth=findMaxLineWidth(shape->content);
        int shapeHeight=countLines(shape->content);

        struct ClientRect rect={.x=shape->x,.y=shape->y,.width=shapeWidth,.height=shapeHeight};
        return rect;
    }
    int rectanglesIntersect(const struct ClientRect* rect1, const struct ClientRect* rect2) {
        int x1 = rect1->x;
        int y1 = rect1->y;
        int x2 = x1 + rect1->width;
        int y2 = y1 + rect1->height;

        int x3 = rect2->x;
        int y3 = rect2->y;
        int x4 = x3 + rect2->width;
        int y4 = y3 + rect2->height;

        // Check if one rectangle is to the left of the other
        if (x2 < x3 || x4 < x1) {
            return 0;
        }

        // Check if one rectangle is above the other
        if (y2 < y3 || y4 < y1) {
            return 0;
        }

        return 1; // Rectangles intersect
    }

#endif