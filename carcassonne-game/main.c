#include <stdlib.h>
#include "include/player.h"


int main(void){
    
    stack *S = get_tiles_from_file("data/tuiles_base_simplifiees.csv");
    print_stack(S);
    print_tile_info(&S->tab[0]);
    free_stack(S);

    return 0;
}