#include <stdlib.h>
#include "include/game.h"
// #include "include/tile.h"


int main(void){
    
    game *GC = start_creative_game();
    creative_gameplay(GC);
    free_creative_game(GC);
    

    return 0;
}