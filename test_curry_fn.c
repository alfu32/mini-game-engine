#include <stdio.h>

// Define a function type that takes an integer and returns an integer
typedef int (*IntFunction)(int);

// Function that takes an integer and returns a function pointer
IntFunction createMultiplier(int factor) {
    // Define and return an actual function
    int multiplier(int x) {
        return x * factor;
    }
    
    return multiplier; // Return a pointer to the function
}

int main() {
    // Create a function that multiplies by 2
    IntFunction timesTwo = createMultiplier(2);
    
    // Use the returned function
    printf("Result: %d\n", timesTwo(5)); // Should print 10
    
    return 0;
}
