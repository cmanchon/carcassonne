#include <stdlib.h>
#include "include/player.h"


int main(void){
    
    stack *S = get_tiles_from_file("data/tuiles_base_simplifiees.csv");
    print_tile_info(&(S->tab[70]));
    // print_stack(S);
    printf("alo : %s\n", S->tab[0].sides[0].type);
    printf("alo : %s\n", S->tab[0].sides[1].type);
    printf("alo : %s\n", S->tab[0].sides[2].type);
    printf("alo : %s\n", S->tab[0].sides[3].type);
    free_stack(S);

    return 0;
}