#include "shape.c"

// Define a function pointer type for shape transformation
typedef Shape (*ShapeTransform)(struct Shape*, int);

// Define a struct for an animated shape
typedef struct AnimatedShape {
    struct Shape* shape;
    int birth_frame;
    int isAlive;
    ShapeTransform next;
} AnimatedShape;



// Function to create an animated shape
AnimatedShape* animated_shape_new(int birthFrame,int x0, int y0, const char* content, ShapeTransform transformFunc) {
    AnimatedShape* animatedShape=(AnimatedShape*)malloc(sizeof(AnimatedShape));
    animatedShape->shape = shape_new(x0,y0,content);
    animatedShape->birth_frame = birthFrame;
    animatedShape->isAlive = 1;
    animatedShape->next = transformFunc;
    return animatedShape;
}

// Function to destroy an animated shape
void animated_shape_dealloc(AnimatedShape* animatedShape) {
    if (animatedShape->shape != NULL) {
        shape_dealloc(animatedShape->shape);
        free(animatedShape->shape);
    }
    animatedShape->shape = NULL;
    free(animatedShape);
}

