#include <stdio.h>

int main(void){
    int tmp;
    scanf("%d", &tmp);
    if (tmp == 0) printf("omg\n");
    printf("tmp : %d\n", tmp);
    char test = (char) tmp;
    char test2 = tmp + '0';
    printf("test : %c\n", test); 
    printf("test2 : %c\n", test2); 
    if (test == 'Q') printf("omg2\n");
    if (test2 == 'Q') printf("omgt2\n");

    return 0;
}