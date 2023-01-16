#include <stdlib.h>
#include "include/tile.h"
#include "include/player.h"


int main(void){
    tile *T = init_tile("pre", "route", "pre", "route", "route", 1);
    print_tile_info(T);
    free_tile(T);

    return 0;
}