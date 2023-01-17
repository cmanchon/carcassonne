#include "../include/tile.h"

//TILE
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
    printf("tile freed.\n"); 
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


//STACK
stack *init_stack() {
  stack *S = NULL;
  S = (stack *)malloc(sizeof(stack));
  S->tab = (tile *)malloc(sizeof(tile));
  S->nb_tiles = 0;
  return S;
}


void free_stack(stack *S){
    free(S->tab);
    free(S);
    printf("stack freed.\n");
}

tile* pop(stack *S){
    if (S->nb_tiles == 0){
        return NULL;
    }
    else{
        tile *L = &(S->tab[S->nb_tiles-1]);
        tile* L_copy = init_tile(L->sides[0].type, L->sides[1].type, L->sides[2].type, L->sides[3].type, L->sides[4].type, L->id);
        L = NULL;
        S->tab = realloc(S->tab, (S->nb_tiles - 1) * sizeof(tile));
        S->nb_tiles--;
        return L_copy;
    }
}

void push(stack *S, tile *T) {
  S->tab = realloc(S->tab, (S->nb_tiles + 1) * sizeof(tile));
  S->tab[S->nb_tiles] = *T;
  S->nb_tiles++;
}


void print_stack(stack *S){
    if (S->nb_tiles == 0){
        printf("Stack empty.\n");
    }
    else{
        for (int i = 0 ; i < S->nb_tiles ; i++){
            print_tile_info(&S->tab[i]);
            printf("\n");
        }
    }
    
}


stack* get_tiles_from_file(char* filename){
    FILE *fh = fopen(filename, "r");

    if (fh == NULL){
        printf("get_tiles_from_file : erreur d'ouverture de %s\n", filename);
        exit(1);
    }
    stack* S = init_stack();
    size_t size;
    char *buf = NULL;
    char **types = (char**)malloc(5*sizeof(char*));
    for (int j = 0 ; j < NB_OF_TILES ; j++){
        for (int i = 0 ; i < 5 ; i++){
            if (i==4){
                getdelim(&buf, &size, '\n', fh);
            }
            else{
                getdelim(&buf, &size, ',', fh);
            }
            types[i] = (char*)malloc(20*sizeof(char));
            buf[strlen(buf)-1] = '\0';
            strcpy(types[i], buf);
        }
        tile* tmp = init_tile(types[0], types[1], types[2], types[3], types[4], j+1);
        // push(*S, tmp);
        // print_tile_info(&S->tab[j]);
        print_tile_info(tmp);
        printf("\n\n");
        free_tile(tmp);
    }



    fclose(fh);
    for (int i = 0 ; i < 5 ; i ++){
        free(types[i]);
    }
    free(types);

    return NULL;
}



//GRID
grid* init_grid(){
    grid* G = (grid*)malloc(sizeof(grid));
    G->tab  = (tile**)malloc(NB_OF_TILES*2-1 * sizeof(tile*));
    if (G->tab != NULL){
        int i = 0;
        do
        {
            G->tab[i] = (tile*)malloc(NB_OF_TILES*2-1 * sizeof(tile));
            i++;
        } while (i < NB_OF_TILES*2-1 && G->tab[i-1] != NULL);
        
    }

    G->nb_tiles = 0;
    return G;
}


void free_grid(grid *G){
    for (int i = 0 ; i < NB_OF_TILES*2-1 ; i++){
        free(G->tab[i]);
    }
    free(G->tab);
}