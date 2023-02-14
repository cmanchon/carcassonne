#include <stdlib.h>
#include "include/game.h"
// #include "include/tile.h"


int main(void){
    
    //tile* T = init_tile('b', 'b', 'p', 'c', 'c', 1);

    // T->sides[0].meeple = 0;
    // tile* M = init_tile('c', 'p', 'c', 'b', 'p', 2);
    // tile* O = init_tile('r', 'r', 'c', 'c', 'b', 3);
    // O->sides[2].meeple = 4;
    // O->sides[1].meeple = 3;
    // tile* E = init_tile('p', 'r', 'r', 'c', 'b', 4);


    //grid *G = init_grid();
    
    //stack *S = get_tiles_from_file("data/tuiles_base_simplifiees.csv");

    
    //place_tile_on_grid(G, T, 72, 72, 12);
    //print_grid(G, 0, 1);

    //free_grid(G);

    // free_tile(M);If prompted, confirm access to you

    
    game *G = start_game("data/tuiles_base_simplifiees.csv");
    deal_cards(G);
    printf("player 0 hand : \n");
    print_stack(G->players[0]->hand);
    free_game(G);
    

    
    // stack *S = get_tiles_from_file("data/test.csv");
    // print_stack(S);
    // printf("before : %d\n", S->nb_tiles);
    // printf("-------------------------------------------------------------------\n\n");
    // shuffle(S);
    // print_stack(S);
    // printf("after: %d\n", S->nb_tiles);
    // free_stack(S);
    
    return 0;
}