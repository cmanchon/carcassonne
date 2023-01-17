#include <stdlib.h>
#include "include/tile.h"
#include "include/player.h"


int main(void){
    tile *T = init_tile("pre", "route", "pre", "route", "route", 1);
    stack *S = init_stack();
    // print_tile_info(T);
    // print_stack(S);
    push(S, T);
    printf("print_stack dans main :\n");
    print_stack(S);
    free_tile(T);
    free_stack(S);

    // get_tiles_from_file("data/tuiles_base_simplifiees.csv");

    return 0;
}