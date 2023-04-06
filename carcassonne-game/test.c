#include <stdio.h>
#include <stdlib.h>

void f(int tab[2]){
    tab[0]++;
    tab[1] = 12;
}


int main(void){
    int t[2] = {0, 0};
    printf("(%d ; %d)\n", t[0], t[1]);
    f(t);
    printf("(%d ; %d)\n", t[0], t[1]);
    return 0;
}