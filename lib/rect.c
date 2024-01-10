#ifndef RECT_C
#define RECT_C
    #include "rect.h"
    #include <string.h>
    #include <stdlib.h>
    #include <stddef.h>
    int rectangle__static__between(int x, int a, int b) {
        return a<=x && x<=b;
    }
    int rectangle__contains_point(const struct rectangle_t* rect1, int x3, int y3) {
        int x1 = rect1->x;
        int y1 = rect1->y;
        int x2 = x1 + rect1->width;
        int y2 = y1 + rect1->height;
        return rectangle__static__between(x3,x1,x2) && rectangle__static__between(y3,y1,y2);
    }
    int rectangle__intersects(const struct rectangle_t* rect1, const struct rectangle_t* rect2) {
        int x1 = rect1->x;
        int y1 = rect1->y;
        int z1 = rect1->z;
        int x2 = x1 + rect1->width;
        int y2 = y1 + rect1->height;

        int x3 = rect2->x;
        int y3 = rect2->y;
        int z3 = rect2->z;
        int x4 = x3 + rect2->width;
        int y4 = y3 + rect2->height;
        
        return z1 == z3 && (
            rectangle__contains_point(rect1,x3,y3) 
            || rectangle__contains_point(rect1,x4,y3) 
            || rectangle__contains_point(rect1,x4,y4) 
            || rectangle__contains_point(rect1,x3,y4) 
            || rectangle__contains_point(rect2,x1,y1) 
            || rectangle__contains_point(rect2,x2,y1) 
            || rectangle__contains_point(rect2,x2,y2) 
            || rectangle__contains_point(rect2,x1,y2)
            );
    }

    int rectangle__free(struct rectangle_t* rect1){
        free(rect1);
        return 0;
    }

#endif