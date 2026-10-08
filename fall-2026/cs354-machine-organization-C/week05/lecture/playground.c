#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    
    int headers[] = {33, 49, 64, 80};
    if( headers[0] & 1 ) printf("[0]: is allocated\n");
    if( headers[1] % 2 ) printf("[1]: is allocated\n");
    headers[2] = headers[2] | 1; // change to allocated
    headers[3] = headers[3] + 1; // change to allocated
    if( headers[2] & 1 ) printf("[2]: is now allocated\n");
    if( headers[3] % 2 ) printf("[3]: is now allocated\n");
        
    return 0;
}