#include "../include/tile.h"


tile* init_tile(char* side_A, char* side_B, char* side_C, char* side_D, char* side_E, int id){
    tile *T = (tile*)malloc(sizeof(tile)); 
    T->sides = (side*)malloc(5*sizeof(side)); 
    
    T->id = id;

    T->sides[0].type = side_A; 
    T->sides[0].meeple = UND; 
    
    T->sides[1].type = side_B; 
    T->sides[1].meeple = UND; 
    
    T->sides[2].type = side_C; 
    T->sides[2].meeple = UND; 
    
    T->sides[3].type = side_D; 
    T->sides[3].meeple = UND; 
    
    T->sides[4].type = side_E; 
    T->sides[4].meeple = UND; 

    T->state = 0;
    T->played_by = UND;
    T->x = UND;
    T->y = UND;
    if (strcmp(side_A, "blason")==0 || strcmp(side_B, "blason")==0 || strcmp(side_C, "blason")==0 || strcmp(side_D, "blason")==0 || strcmp(side_E, "blason")==0 ) T->blason = 1;
    else T->blason = 0;

    return T;
}


void free_tile(tile *T){
    free(T->sides);
    free(T);    
}

void print_tile_info(tile *T){
    printf("Tuile %d :\n", T->id);

    printf("  %c  \n", T->sides[0].type[0]);
    printf("%c %c %c\n", T->sides[3].type[0], T->sides[4].type[0], T->sides[1].type[0]);
    printf("  %c  \n\n", T->sides[2].type[0]);

    printf("state : %d\n", T->state);
    printf("played_by : %d\n", T->played_by);
    printf("coords : (%d, %d)\n", T->x, T->y);
    printf("blason : %d\n", T->blason);
}