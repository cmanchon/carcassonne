#include <stdlib.h>
#include "include/tile.h"
#include "include/player.h"


int main(void){
    tile *T = init_tile("pre", "route", "pre", "route", "route", 1);
    print_tile_info(T);
    free_tile(T);

    get_tiles_from_file("data/tuiles_base_simplifiees.csv");


    return 0;
}