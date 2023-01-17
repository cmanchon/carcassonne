#include <stdlib.h>
#include "include/tile.h"
#include "include/player.h"


int main(void){
    tile *T = init_tile("pre", "route", "pre", "route", "route", 1);
    tile *T2 = init_tile("pre", "ville", "route", "route", "route", 2);
    stack *S = init_stack();
    push(S, T);
    push(S, T2);
    printf("1. print_stack dans main :\n");
    print_stack(S);

    tile *tmp = pop(S);
    printf("2. print_stack dans main :\n");
    print_stack(S);
    printf("\ntile popped : \n");
    print_tile_info(tmp);
    free_tile(tmp);
    free_tile(T);
    free_tile(T2);
    free_stack(S);

    // get_tiles_from_file("data/tuiles_base_simplifiees.csv");

    return 0;
}