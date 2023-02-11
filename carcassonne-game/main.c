#include <stdlib.h>
//#include "include/player.h"
#include "include/tile.h"


int main(void){
    
    tile* T = init_tile('b', 'b', 'p', 'c', 'c', 1);
    tile* M = init_tile('c', 'p', 'c', 'p', 'p', 2);
    tile* O = init_tile('r', 'r', 'c', 'c', 'b', 3);

    grid *G = init_grid();
    
    place_tile_on_grid(G, *T, 17, 20, 12);
    place_tile_on_grid(G, *M, 18, 20, 12);
    place_tile_on_grid(G, *M, 15, 25, 12);
    place_tile_on_grid(G, *O, 19, 21, 12);
    place_tile_on_grid(G, *O, 19, 20, 12);
    // print_tile_info(&G->tab[0][0]);


    print_grid(G);

    free_grid(G);
    free_tile(T);
    free_tile(M);
    free_tile(O);
    return 0;
}