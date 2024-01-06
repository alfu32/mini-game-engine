#ifndef DRAW_C
#define DRAW_C
    #include "draw.h"
    #include "viewport.h"
    #include "shape.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    // Function to clear the entire terminal screen
    void terminal_clear() {
        printf("\e[1;1H\e[2J");
    }
    // Function to draw a shape onto the screen buffer
    void viewport_shape_draw(Viewport* screen, Shape* shape,char fg,char bg) {
        int y = shape->y;
        int x = shape->x;
        int offset_y=0;
        char* content = shape->content;

        while (*content) {
            if(*content != BLANK){
                if( *content == '\n' ){
                    offset_y+=1;
                    x=shape->x;
                } else {
                    viewport_draw_char(screen, x, y+offset_y, *content,fg,bg);
                }
            }
            content++;
            x++;
        }
    }

    // Function to split a multiline string into an array of lines
    char** split_lines(const char* input, int* lineCount) {
        if (input == NULL) {
            return NULL;
        }

        // Count the number of lines
        int count = 0;
        const char* p = input;
        while (*p) {
            if (*p == '\n') {
                count++;
            }
            p++;
        }
        count++; // Add one for the last line (no trailing newline)

        // Allocate memory for the array of lines
        char** lines = (char**)malloc(count * sizeof(char*));
        if (lines == NULL) {
            return NULL;
        }

        // Initialize the array of lines
        p = input;
        int i = 0;
        while (*p) {
            const char* lineStart = p;
            while (*p && *p != '\n') {
                p++;
            }
            size_t lineLength = p - lineStart + 1; // Include the newline character
            lines[i] = (char*)malloc(lineLength);
            if (lines[i] == NULL) {
                // Memory allocation failed
                for (int j = 0; j < i; j++) {
                    free(lines[j]);
                }
                free(lines);
                return NULL;
            }
            strncpy(lines[i], lineStart, lineLength);
            lines[i][lineLength - 1] = '\0'; // Null-terminate the line
            i++;
            if (*p == '\n') {
                p++;
            }
        }

        *lineCount = count;
        return lines;
    }

    // Function to rotate a line circularly
    char* circular_offset_line(char* line) {
        char * offset_line=strdup(line);
        if (line == NULL || *line == '\0') {
            return line;
        }
        size_t l=strlen(line);
        /// char last_char = line[0];
        /// for (int i = l - 1; i > 0; i--) {
        ///     line[i] = line[i - 1];
        /// }

        char last_char = offset_line[0];
        for ( int i=0;i<(l-2);i++){
            offset_line[i] = line[i+1];
        }
        offset_line[l-2] = last_char;
        return offset_line;
    }

    // Function to rotate a multiline string
    char* circular_offset_string(const char* input) {
        if (input == NULL) {
            return NULL;
        }

        int lines_count = 0;
        char** lines = split_lines(input, &lines_count);
        if (lines == NULL) {
            return NULL;
        }

        // Allocate memory for the result
        size_t resultLength = strlen(input) + 1;
        char* result = (char*)malloc(resultLength);
        if (result == NULL) {
            // Memory allocation failed
            for (int i = 0; i < lines_count; i++) {
                free(lines[i]);
            }
            free(lines);
            return NULL;
        }

        // Build the rotated multiline string
        // result[0] = '\0';
        for (int i = 0; i < lines_count; i++) {
            char * ofl=circular_offset_line(lines[i]);
            strcat(result, ofl);
            if (i < lines_count - 1) {
                strcat(result, "\n");
            }
            free(ofl);
            free(lines[i]);
        }
        free(lines);

        return result;
    }
#endif