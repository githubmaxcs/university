# Introduction

## Lecture 03 (2026-09-15)

Note: Obtain the course contents in [GitHub](https://git.doit.wisc.edu/DAHL/cs354-outlines/-/tree/main?ref_type=heads). 

The lecture.md files are lecture outlines with personal notes. The html files serve as a blank template when looking back through material.

### Lecture Overview

- Learning Plan
- C Pointers
- Virtual Memory Organization
- Multidimensional Arrays

### C Pointers (Basics Operators)

Every variable can be associated with a virtual memory address.

```C
int x = 123;
printf("value of x: %d\n", x);
printf("the memory address of x: %p\n", &x); // & operator gets address
```

Memory addresses can be stored in variables.

```C
int x = 123;
int* y = &x; // type int* holds an address of an int (called a pointer)
printf("address stored in y: %p\n", y);
printf("value at that address: %d\n", *y); // * operator dereferences
// dereferencing an address/pointer evaluates to the value at that address
```

We can get and dereference pointers to any type (including pointers).

```C
int x = 123;
int* y = &x; // read types from right to left
int** z = &y; // * is read as "pointer to"
printf("x holds the value: %d\n", x);
printf("y points to the value: %d\n", *y);
printf("z points to the pointer that points to the value: %d\n", **z);
```

### Practice with Pointers and Memory Diagrams

Fill in the `???` below so that these code and memory diagrams are consistent:

```C
int a = 33;  // printf("%p",&a); ==> 0x...FC
int b = 35;   // printf("%p",&b); ==> 0x...F8
int* c = &a;  // printf("%p",&c); ==> 0x...F4
int* d = &b; // printf("%p",&d); ==> 0x...F0
```
| address | data               |
| ------- | ------------------ |
| 0x…FC   | 0x0000000000000021 |
| 0x…F8   | 0x0000000000000023 |
| 0x…F4   | 0x…FC              |
| 0x…F0   | 0x…F8              |

### Practice with Pointers and Correctly Matching Types

Note that `int* a, b = 354;` is equivalent to two lines `int* a;` and `int b = 354`.

```C
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
```

A segmentation fault (often called a segfault) is a specific type of runtime error that happens when a program tries to access or modify a memory area it is not allowed to touch. Here are some common causes:
- Dereferencing a NULL pointer: Trying to read or write using a pointer that points to memory address 0 (which is never valid).
- Using uninitialized pointers (Wild pointers): Declaring a pointer without setting it to a valid address, meaning it points to random, unauthorized memory.
- Accessing freed memory (Dangling pointer): Trying to use a memory block via a pointer after it has already been released using free().
- Buffer overflows / Out-of-bounds array access: Reading or writing past the legal index limits of an array (e.g., accessing arr[10] when an array only has 5 elements).
- Writing to read-only memory: Attempting to change a string literal (like char *str = "hello"; str[0] = 'H';) which is stored in a protected segment of memory.
- Stack overflow: Consuming all available stack memory, usually caused by infinite recursive function calls without a base case.

### Heap Memory

Be careful about returning pointers to variables on the stack.

```C
int* foo() {
  int x = 3;
  return &x; // fortunately this kind of mistake does produce warnings
}
int* bar() {
  int y = 4;
  return &y; // but it also produces running code (see below)
}
int main(int argc, char** argv) {
  int* a = foo();
  int* b = bar();
  printf("*a: %d, *b: %d\n", *a, *b);
}
```

| Stack |
| ----- |
| main: argc: 1 <br> argv: {"a.out"} <br> a <br> b |
| ______ |
| ~~foo: x = 3~~ (<--a) | 
| ~~bar: y = 4~~ (<--b) |

When a function returns, we pop the memory allocations off the stack. So, when `a` is initialized to a pointer to `foo()` but `foo()` returns, we are left with a dangling pointer. So, it's a pointer to an address of memory that we're no longer using. The same goes for `b` and `foo()`.

In the case above the output is `*a: 4, *b: 4`. The memory for `x` is cleared and the machine then allocates the memory for `y`. So, `a` and `b` are pointing to the same memory address.

Any function can dynamically allocate memory on the heap.

```C
int* createInt(int initialValue) {
  int* x = malloc(sizeof(int));
  *x = initialValue;
  return x;
}
int main(int argc, char** argv) {
  int* a = createInt(5);
  int* b = createInt(6);
  printf("*a: %d, *b: %d\n", *a, *b);
  free(a);
  free(b);
}  
```
| Stack | Heap |
| ----- | ---- |
| main: argc: 1 <br> argv: {"a.out"} <br> a <br> b | Block of 4 Bytes Storing 5 (<--x) |
| ________________________________________________ | Block of 4 Bytes Storing 6 (<--x) |
| ~~createInt: <br>initValue 5 <br> x~~ | 
| ~~createInt: <br>initValue 6 <br> x~~ | 

But you are then responsible for later free()ing the memory.

Additional uses for Dynamically Allocated Heap Memory:
- The size of your memory needs are not clear until runtime.
- The size of your memory needs are too big to fit on the stack.
- The amount of memory you are using grows and shrinks over time.

### Virtual Memory Org (Lifetimes Code)

```C
int x = 1;
int foo(int a) {
  static int x = 2;
  return x = x + a;
}
int bar(int x) {
  return foo(x+3) + x;
}
int main(int argc, char** argv) {
  x = bar(x);
  for(int x=0;x<4;x++)
    x = foo(x);
  printf("%d\n",x);
}
```

Output: 7.

### Virtual Memory Org (Layout)

| Part | Address |
| ---- | ------- |
|      | 0xFFFFFFFFFFFFFFFF |
|      | 0x0000FFFFFFFFFFFF |
|  LINUX KERNEL | |
| Stack | local functions and scoped variables |
| ↓ | |
| | |
| ↑ | |
| Heap | malloc()ed dynamically |
| Data | global and static variables |
| .bss | zero or uninitialized |
| .data | initialized to non-zero value |
| Code | |
| .rodata | string constants or other big constants |
| .text | instructions of our program |
| RESERVED | 0x0000000000000000 |

Note that these virtual addresses are managed (remapped) by the OS.

### Virtual Memory Org (Practice)

```C
char* x = "a happy little string";
int main(int argc, char** argv) {
  int length = strlen(x) + 1;
  char* y = malloc(sizeof(char) * length);
  strncpy(y,x,length);
  printf("%s\n", y);
  free(y);
}
```

Add expressions below that evaluate to values in each memory segment.

| Memory | SegmentExpression |
| ------ | ----------------- |
| Stack | &y, &length |
| Heap | y |
| Data | &x |
| Code | x |

How can you check whether these expressions are correctly categorized? Well, you can print the memory addresses of the code to check whether the segmentation of the memory addresses.

```C
#include <string.h>

char* x = "a happy little string";
int main(int argc, char** argv) {
  int length = strlen(x) + 1;
  char* y = malloc(sizeof(char) * length);
  strncpy(y,x,length);
  printf("%s\n", y);
  printf("stack: %p\n", &y); 
  printf("heap: %p\n", y); 
  printf("data: %p\n", &x); 
  printf("code: %p\n", x); 
  free(y);
}
```

### Memory Organization (Best Practices)
- Generally prefer stack memory when lifetimes match function.
- Use global/static memory when it must persist entire program duration.

Extra control over lifetime of dynamic heap memory:
- It has extra time and space over head costs (we’ll explore more next unit).
- It requires programmer to be responsible about free()ing to avoid leaks.

Memory Organization (Zooming in)

Size of native types in C:

```C
printf("char:    %lu\n",sizeof(char));  // single byte
printf("short:   %lu\n",sizeof(short)); // small int
printf("int:     %lu\n",sizeof(int));   // normal int
printf("long:    %lu\n",sizeof(long));  // big int
printf("float:   %lu\n",sizeof(float)); // normal float
printf("double:  %lu\n",sizeof(double));// big float
printf("pointer: %lu\n",sizeof(void*)); // pointer
```
Results:\
char:    1\
short:   2\
int:     4\
long:    8\
float:   4\
double:  8\
pointer: 8

Note that on Windows “long” was kept at 4 bytes for backward compatibility.

How does a pointer/address reference values spanning multiple bytes?
- memory is byte addressable
- multi-byte values are contiguous in memory
- pointers reference smallest address of those the value spans
- bytes are organized in big-endian vs little-endian order

Big vs Little Endian
Suppose you want to store the int value 305419896. 
- What is the Hex value corresponding to this number? 0x12345678
- What are the four individual bytes that must be stored?  0x12 34 56 78
- Complete the following diagram, assuming the int is stored at address 0x…F8 using little endian ordering.

| address | data (little endian) | data (bit endian) |
| 0x…FB | 12 | 78 |
| 0x…FA | 34 | 56 |
| 0x…F9 | 56 | 34 |
| 0x…F8 | 78 | 12 |
| 0x…F7 | | |
| 0x…F6 | | |
| 0x…F4 | | |

Big Endian

Definition: The most significant byte (the "big end" or highest-value byte) is stored at the lowest memory address.

Little Endian 

Definition: The least significant byte (the "little end" or lowest-value byte) is stored at the lowest memory address.

The following program stores a 4-byte integer and examines its very first byte in memory. If the first byte is 0x01 (the lowest value), the system is Little Endian. If the first byte is 0x00, it is Big Endian.

```C
#include <stdio.h>

int main() {
    unsigned int num = 1; // Hex representation: 0x00000001
    
    // Point a character pointer (1 byte) to the start of the integer (4 bytes)
    char *byte_ptr = (char*)&num;

    if (*byte_ptr == 1) {
        printf("Your system is **Little Endian**.\n");
    } else {
        printf("Your system is **Big Endian**.\n");
    }

    return 0;
}
```

### Multidimensional Arrays (Pointer Arithmetic)

Arrays are stores as contiguous blocks of memory. Compare the distance between the first two addresses in each of the following arrays, to the size of the values in that array.

```C
short shorts[4];
int ints[4];
long longs[4];
printf("shorts: %p vs %p\n", &(shorts[0]), &(shorts[1]));
printf("ints: %p vs %p\n", &(ints[0]), &(ints[1]));
printf("longs: %p vs %p\n", &(longs[0]), &(longs[1]));
```

And really the single dimensional array is tracked using the address of the first value inside of it:

```C
int ints[4] = {7,8,9,10};
printf("%p == %p\n", ints, &(ints[0]));
printf("%d == %d\n", *ints, ints[0]);
```

So the indexing notation `ints[x]` is really just performing arithmetic:

```C
int ints[6] = {4,5,6,7,8,9};
printf("%d == %d\n", ints[2], *(ints + ???));
```

Note that a scale factor is being applied for you. In this case an int is 4 bytes.

```C
short shorts[] = {1,2,3};
int ints[] = {4,5,6,7};
long longs[] = {8,9,10,11,12};
// try printing out the values 2, 7, and 10 from the above arrays,
// WITHOUT using any [] square brackets.
printf("%d\n", *(shorts + 1));
printf("%d\n", *(ints + 3));
printf("%ld\n", *(longs + 2));
```

![alt text](array2d.png)

```C
int *a = (int *)malloc(5 * sizeof(int));; // initialize a as array that makes use of heap memory
int b[5]; // initialize b as array that makes use of stack memory
// and dont't forget to free a when you are done
free(a);
```