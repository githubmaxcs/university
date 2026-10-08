#include <stdio.h>
#include <stdlib.h>

int main() {
    int x = 123;
    printf("value of x: %d\n", x);
    printf("the memory address of x: %p\n", &x); // & operator gets address

    // type int* holds an address of an int (called a pointer)
    // read types from right to left
    int* y = &x;
    printf("address stored in y: %p\n", y);
    printf("value at that address: %d\n", *y); // * operator dereferences
    // dereferencing an address/pointer evaluates to the value at that address

    int** z = &y; // * is read as "pointer to"
    printf("x holds the value: %d\n", x);
    printf("y points to the value: %d\n", *y);
    printf("z points to the pointer that points to the value: %d\n", **z);

    int* a, b = 354;
    // 1. print out the value of b through the pointer a
    a = &b;
    printf("Value of b through pointer a: %d\n", *a);
    // 2. what if a is initialized to NULL before this print statement?
    // // Answer: We get a segmentation fault.
    // a = NULL;
    // printf("Value of b through pointer a: %d\n", *a);
    // 3. create and initialize variable c to hold the address of a
    int** c = &a;
    // 4. print out the value of b through the pointer c
    printf("Value of b through pointer c: %d\n", **c);
    
    return 0;
}