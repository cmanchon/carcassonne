#include <stdlib.h>
#include "include/game.h"
// #include "include/tile.h"


int main(void){
    
    //TEST GAME
    game *G = start_game("data/tuiles_base_simplifiees.csv");
    gameplay(G);
    free_game(G);

    //TEST CREATIVE GAME
    // game *GC = start_creative_game();
    // creative_gameplay(GC);
    // free_creative_game(GC);
    

    
    return 0;
}