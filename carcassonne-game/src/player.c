#include "../include/player.h"

player* init_player(int numero, int meeple){
    player *P = (player*)malloc(sizeof(player));

    P->id = numero;
    P->meeple_color = meeple;
    P->meeple_number = 7;
    P->score = 0;

    P->hand = init_stack();

    return P;
}


void free_player(player *P){
    free_stack(P->hand);
    free(P);
}


void print_player(player *P){
    printf("Player %d\nmeeples : ", P->id);
    printf("\033[");
    switch (P->meeple_color)
        {
        case 0:
            printf("33m");          //yellow
            break;
        
        case 1:
            printf("31m");          //red
            break;
        
        case 2:
            printf("32m");          //green
            break;
        
        case 3:
            printf("34m");          //blue
            break;
        
        case 4:
            printf("30m");          //black
            break;
        
        default:
            printf("37m");          //white
            break;
        }

    printf(" %d \033[0m\n", P->meeple_number);

    printf("score : %d\n", P->score);
    printf("%d tiles in hand\n", P->hand->nb_tiles);

}