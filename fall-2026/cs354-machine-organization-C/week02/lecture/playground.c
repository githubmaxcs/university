#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int x = rand() % 2; // randomly assigns either 0 or 1 to x
    printf("%d: ", x);
    if(x) {printf("rand() is odd\n\n");} else {printf("rand() is even\n\n");}

    if(-1) printf("-1 is true\n");
    if('0') printf("'0' is true\n");
    if('\0') printf("'\\0' is true\n");
    if(NULL) printf("NULL is true\n");

    size_t definedSize = 6;
    int array[definedSize];
    size_t computedSize = sizeof(array) / sizeof(int);
    printf("\nlength: %lu == %lu\n", definedSize, computedSize);

    char y = 127;
    char array_test[6];  
    for(int i=0;i<=6;i++) array_test[i] = 0;
    printf("\ny is now %d\n",y);

    int money = -11;
    if ((money = 0)) printf("your account is empty\n");
    else if(money < 0) printf("you are in debt\n");
    else printf("you're rich\n\n");

    char course[6] = "CS354";
    if( course[5] ) printf("true"); else printf("false");

    return 0;
}