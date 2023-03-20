#include <stdlib.h>
#include "include/game.h"
// #include "include/tile.h"


int main(void){
    
    game *G = start_game("data/tuiles_base_simplifiees.csv");
    print_players_hand(G->players[0]);
    gameplay(G);
    free_game(G);

    return 0;
}