# Introduction

## Lecture 02 (2026-09-03)

Note: Obtain the course contents in [GitHub](https://git.doit.wisc.edu/DAHL/cs354-outlines/-/tree/main?ref_type=heads). 

The lecture.md files are lecture outlines with personal notes. The html files serve as a blank template when looking back through material.

### Lecture Overview

- Course Introduction (Q&A)
- Finish Bash Introduction
- C Introduction
- C Pointers

### Bash Introduction (Continued)

Output of command: `tree .`

```
├── oldDirectory
│   └── backup.txt
└── welcome.txt
```

What does the command `tree .` output when the following command are run?

```
cp welcome.txt oldDirectory/welcome2.txt
mv oldDirectory/backup.txt newDirectory
```

Answer:

```
├── oldDirectory
│   └── welcome2.txt
├── newDirectory
└── welcome.txt
```

#### Bash (Modify Files and Directories)
`cp` - CoPy

`cp -r`

`scp`: `src` or `dst` can be on another computer.

`mv` - MoVe

`rm` - ReMove

`rmdir`: ReMove DIRectory

`rm -rf`

`gio trash`: moves files or directories safely to the system Trashcan in Linux

### C Introduction (Hello, World!)

```C
public class Main {
    public static void main(String[] args) {
	System.out.println("Hello, World!");
    }
}
```

```C
#include <stdio.h>
int main(int argc, char** argv) {
  printf("Hello, World!\n");
} 
```

### C Introduction (Compiling and Running)

```bash
gcc -Wall -std=gnu99 source.c # creates executable file a.out
gcc -Wall -std=gnu99 source.c -o executable
```

- `Wall` : sets Warnings level to ALL
- `std=gnu99` : sets compiler to use c99 standard with some gnu features

**Run that program from the command line with args a b c**

```bash
./a.out a b c
./executable a b c
```

### C Introduction (Java Differences)

  main() is not in a class, and there are not any classes in C
  import statements are effectively #include
  lack of string concatenation everywhere, instead use printf
  historic lack of boolean types, so false == zero-ish values
  arrays do not track their own length and are not bounds checked
  c-strings are null terminated character arrays
  we’ll circle back to pointers and memory management for Unit2

### C Introduction (printf)
Note that Java has System.out.printf, and printf is in bash too.\
`System.out.println(“Hello” + “, ” + “World” + “!”);`\
  vs\
`System.out.printf(“%s, %s!\n”,”Hello”,”World”);`\
It's a lower-level and more efficient expression of desired computation. But it requires knowing placeholders for different types:
- %d - decimal number (%04d pads with leading 0s to be 4 characters wide)
- %x - hexadecimal number
- %c - character
- %f - float (%lf for long float = double)
- %s - string
- %p - pointer / memory address
- %lu - long unsigned integers (often useful for sizes)

### Personal Notes (Extra)

Touch up on hexidecimal.

### C Introduction (Lack of Boolean Types)

Integers are used to encode boolean values, so this is ok:

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int x = rand() % 2; // randomly assigns either 0 or 1 to x
    printf("%d: ", x);
    if(x) {printf("rand() is odd");} else {printf("rand() is even");}
    return 0;
}
```

And this does not create a syntax error like it does in Java. Here's another example:

```C
int money = -11;
if ((money = 0)) printf("your account is empty\n");
else if(money < 0) printf("you are in debt\n");
else printf("you're rich\n");
```

`money = 0` evaulates the variable `money` to `0`, which also evaulates to be false.

Like boolean values, character values are also just integers:

```C
char x = 'A';
printf("char %c == dec: %d\n", x+1, x+1); 
```

Here are some trickier edge cases to keep in mind:

```C
if(-1) printf("-1 is true\n");
if('0') printf("'0' is true\n");
if('\0') printf("'\\0' is true\n");
if(NULL) printf("NULL is true\n");
```

### C Introduction (Arrays Do Not Track Their Own Length)

In C arrays can be on the stack (in Java they require using ‘new’).

Arrays do not keep track own length, and are not bounds checked.

```C
size_t definedSize = 6;
int array[definedSize];
size_t computedSize = sizeof(array) / sizeof(int);
printf("length: %lu == %lu\n", definedSize, computedSize);
```

It’s up to you to safely index values within array bounds

```C
char x = 127;
char array[6];  
for(int i=0;i<=6;i++) array[i] = 0;
printf("x is now %d\n",x);
```

Possible results of writing beyond the bounds of an array:
- silently changes memory that is not being used for anything else
- corrupts memory that is being used for something else (😳 yikes!)
- Segmentation Fault: crash from attempt to access restricted memory

### C Introduction (c-strings)

c-strings are character arrays terminated by a \0 character. Don’t forget this when computing the memory needed for an c-string.

```C
char course[6] = "CS354";
if( course[5] ) printf("true"); else printf("false");
```

You can read in c-strings from the user using gest, scanf, or fgets.

```C
size_t bufferSize = 10;
char buffer[bufferSize];
prinf("Enter a string (to be read with gets): ");
gets(buffer);
prinf("Enter a string (to be read with scanf): ");
scanf("%9s",buffer);
prinf("Enter a string (to be read with fgets): ");
fgets(buffer, bufferSize, stdin)
```

In this course we'll mostly use `fgets`.

Tracking versus computing the length of a string array:

```C
char string[6] = "cs354";
printf("%s has (strlen) length %lu\n", string, strlen(string));
printf("%s has (sizeof) length %lu\n", string, sizeof(string)/sizeof(char));
```

### C Pointers (Basics Operators)

Every variable can be associated with a virtual memory address.

```C
int x = 123;
printf("value of x: %d\n", x);
printf("the memory address of x: %p\n", &x); // & operator gets address
```