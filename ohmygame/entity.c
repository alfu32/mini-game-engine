#ifndef ENTITY_C
#define ENTITY_C
    #include "include/entity.h"
    #include "include/shape.h"
    #include <stdlib.h>
    #include <stdarg.h>

    // Function to create an animated shape
    entity_t* entity__new(int birthFrame,int x0, int y0, const char* content, ShapeTransform transformFunc,const char color,const char background) {
        entity_t* entity=(entity_t*)malloc(sizeof(entity_t));
        entity->type = SHIP;
        entity->shape = shape__new(x0,y0,content);
        entity->birth_frame = birthFrame;
        entity->life = 1;
        entity->power=1;
        entity->collision=1;
        entity->color=color;
        entity->background=background;
        entity->next = transformFunc;
        return entity;
    }

    // Function to destroy an animated shape
    void entity__dealloc(entity_t* entity) {
        if (entity->shape != NULL) {
            shape__dealloc(entity->shape);
            free(entity->shape);
        }
        entity->shape = NULL;
        free(entity);
    }
    // const char* escapes="\r\n\t\b\"";
    char* escape_chars(char *s) {
        char* str=strdup(s);
        size_t len = strlen(str);
        size_t result_len=len;
        for (size_t i = 0; i < len; i++) {
            if (s[i] == '\r' || s[i] == '\n' || s[i] == '\t' || s[i] == '\b') result_len+=2;
        }
        str = (char*)realloc(str,sizeof(char)*result_len+1);
        for (size_t i = 0; i < len; i++) {
            if (s[i] == '\r' || s[i] == '\n' || s[i] == '\t' || s[i] == '\b') {
                // Replace CR with '\r'
                memmove(&str[i + 2], &str[i], len - i);
                str[i] = '\\';
                str[i+1] = '\\';
                switch (str[i]){
                    case '\r': str[i + 2] = 'r';break;
                    case '\n': str[i + 2] = 'n';break;
                    case '\t': str[i + 2] = 't';break;
                    case '\b': str[i + 2] = 'b';break;
                }
                len+=2;
                i+=2;
            }
        }
        return str;
    }
    void entity__print(entity_t* entity) {
        char* shape_escaped_content = escape_chars(entity->shape->content);
        //escape_chars(shape_escaped_content);

        printf("Entity { type : %d, birth_frame : %d, life : %d, power : %d, collision : %d, team : %d, color : %d, background : %d,\n\
            shape: shape_t { x : %d, y : %d, z : %d, content : \"%s\"\n\
            },\n\
        }\n",
            entity->type,
            entity->birth_frame,
            entity->life,
            entity->power,
            entity->collision,
            entity->team,
            entity->color,
            entity->background,
            entity->shape->x,
            entity->shape->y,
            entity->shape->z,
            entity->shape->content
        );
        free(shape_escaped_content);
    }
#endif
