#include <stdlib.h>
#include "include/game.h"
// #include "include/tile.h"


int main(void){
    
    // tile* T = init_tile('b', 'b', 'p', 'c', 'c', 1);

    // T->sides[0].meeple = 0;
    // tile* M = init_tile('c', 'p', 'c', 'b', 'p', 2);
    // tile* O = init_tile('r', 'r', 'c', 'c', 'b', 3);
    // O->sides[2].meeple = 4;
    // O->sides[1].meeple = 3;
    // tile* E = init_tile('p', 'r', 'r', 'c', 'b', 4);


    // stack *S = get_tiles_from_file("data/tuiles_base_simplifiees.csv");

    // grid *G = init_grid();
    
    // place_tile_on_grid(G, S->tab[0], 100, 100, 12);
    // place_tile_on_grid(G, S->tab[1], 100, 101, 12);
    // place_tile_on_grid(G, S->tab[21], 101, 101, 12);
    // place_tile_on_grid(G, S->tab[26], 101, 100, 12);
    // place_tile_on_grid(G, S->tab[23], 102, 100, 12);
    // place_tile_on_grid(G, S->tab[57], 74, 71, 12);
    // place_tile_on_grid(G, S->tab[61], 73, 71, 12);
    // place_tile_on_grid(G, S->tab[71], 73, 74, 12);
    // place_tile_on_grid(G, S->tab[0], 9, 9, 12);
    // place_tile_on_grid(G, S->tab[1], 9, 10, 12);
    // place_tile_on_grid(G, S->tab[21], 10, 10, 12);
    // place_tile_on_grid(G, S->tab[26], 10, 9, 12);
    // place_tile_on_grid(G, S->tab[23], 11, 9, 12);


    // print_grid(G, 0, 1);

    // free_grid(G);
    // free_tile(T);
    // free_tile(M);
    // free_tile(O);

    // free_stack(S);


    game *G = start_game("data/tuiles_base_simplifiees.csv");
    deal_cards(G);
    free_game(G);

    
    return 0;
}