#include <stdlib.h>
#include "include/game.h"
// #include "include/tile.h"


int main(void){
    
    game *G = start_game("data/tuiles_base_simplifiees.csv");
    gameplay(G);
    free_game(G);

    return 0;
}