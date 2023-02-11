#include "../include/tile.h"

//TILE
tile* init_tile(char side_A, char side_B, char side_C, char side_D, char side_E, int id){
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
    if (side_A == 'b' || side_B == 'b' || side_C == 'b' || side_D == 'b' || side_E == 'b') T->blason = 1;
    else T->blason = 0;

    return T;
}


void free_tile(tile *T){
    free(T->sides);
    free(T);   
}

void print_tile_info(tile *T){
    printf("Tuile %d :\n", T->id);

    printf("  %c  \n", T->sides[0].type);
    printf("%c %c %c\n", T->sides[3].type, T->sides[4].type, T->sides[1].type);
    printf("  %c  \n\n", T->sides[2].type);

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
    for (int i = S->nb_tiles - 1; i >= 0; i--){
        free(S->tab[i].sides);
    }
    free(S->tab);
    free(S);
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

void push(stack *S, tile T) {
    S->tab = realloc(S->tab, (S->nb_tiles + 1) * ( sizeof(tile)+ 5*sizeof(side) ));
    
    S->tab[S->nb_tiles] = T;
    
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
    stack* S = (stack*)malloc(sizeof(stack));
    S->tab = (tile*)malloc(NB_OF_TILES*sizeof(tile));
    for (int i = 0 ; i < NB_OF_TILES ; i++){
        S->tab[i].sides = (side*)malloc(5*sizeof(side));
    }
    S->nb_tiles = NB_OF_TILES;

    size_t size;
    char *buf = NULL;


    
    for (int j = 0 ; j < NB_OF_TILES ; j++){
        S->tab[j].blason = 0;
        for (int i = 0 ; i < 5 ; i++){
            if (i==4){
                getdelim(&buf, &size, '\n', fh);
            }
            else{
                getdelim(&buf, &size, ',', fh);
            }
            if (strcmp(buf, "pre,")==0 || strcmp(buf, "pre\n")==0) S->tab[j].sides[i].type = 'p';
            else if (strcmp(buf, "route,")==0 || strcmp(buf, "route\n")==0) S->tab[j].sides[i].type = 'r';
            else if (strcmp(buf, "ville,")==0 || strcmp(buf, "ville\n")==0) S->tab[j].sides[i].type = 'c';              //pour cité
            else if (strcmp(buf, "village,")==0 || strcmp(buf, "village\n")==0) S->tab[j].sides[i].type = 'v';
            else if (strcmp(buf, "abbaye,")==0 || strcmp(buf, "abbaye\n")==0) S->tab[j].sides[i].type = 'a';
            else if (strcmp(buf, "blason,")==0 || strcmp(buf, "blason\n")==0) {
                S->tab[j].sides[i].type = 'b';
                S->tab[j].blason = 1;
            }
            
            else {
                printf("erreur get_tiles_from_file : erreur de lecture\n");
                printf("%s\n", buf);
                exit(1);
            }
        }
        S->tab[j].id = j+1;
        S->tab[j].played_by = UND;
        S->tab[j].state = 0;
        S->tab[j].x = UND;
        S->tab[j].y = UND;
    }
    free(buf);
    fclose(fh);

    return S;
}



//GRID
grid* init_grid(){
    grid* G = (grid*)malloc(sizeof(grid));
    G->tab  = (tile**)malloc((NB_OF_TILES*2-1) * sizeof(tile*));
    if (G->tab != NULL){
        int i = 0;
        do
        {
            G->tab[i] = (tile*)malloc((NB_OF_TILES*2-1) * sizeof(tile));
            i++;
        } while (i < NB_OF_TILES*2-1 && G->tab[i-1] != NULL);
        
    }

    for (int i = 0 ; i < NB_OF_TILES*2-1 ; i++){
        for (int j = 0 ; j < NB_OF_TILES*2-1 ; j++){
            G->tab[i][j].blason = UND;
            G->tab[i][j].id = UND;
            G->tab[i][j].played_by = UND;
            G->tab[i][j].state = 1;
            G->tab[i][j].x = UND;
            G->tab[i][j].y = UND;

            G->tab[i][j].sides = (side*)malloc(5 * sizeof(side));
            for (int a = 0 ; a < 5 ; a++){
                G->tab[i][j].sides[a].meeple = UND;
                G->tab[i][j].sides[a].type = ' ';
            }
        }
    }

    G->nb_tiles = 0;
    return G;
}


void free_grid(grid *G){
    for (int i = 0 ; i < NB_OF_TILES*2-1 ; i++){
        for (int j = 0 ; j < NB_OF_TILES*2-1 ; j++){
            free(G->tab[i][j].sides);
        }
        free(G->tab[i]);
    }
    free(G->tab);
    free(G);
}


int place_tile_on_grid(grid* G, tile T, int x, int y, int player){
    //returns 1 si could be placed, 0 otherwise

    if (G->tab[x][y].id != UND){
        //there's already a tile at this place
        return 0;
    }

    G->tab[x][y] = T;
    G->tab[x][y].state = 1;
    G->tab[x][y].x = x;
    G->tab[x][y].y = y;
    G->tab[x][y].played_by = player;

    G->nb_tiles++;

    return 1;
}

//Affichage de la grille :
// 
//     0     1     2
//     b
// 0 b b b 
//     b
//     c     p
// 1 c c c p p p
//     c     p
// 
// 2
// 
// 

//couleurs à ajouter

void print_grid(grid *G){
    //on parcourt une première fois pour établir la fenêtre d'où sont les tuiles placées 
    int maxX = -1, maxY = -1;
    int minX = NB_OF_TILES*2, minY = NB_OF_TILES*2;

    for (int i = 0 ; i < NB_OF_TILES*2-1 ; i++){
        for (int j = 0 ; j < NB_OF_TILES*2-1 ; j++){
            if (G->tab[i][j].id != UND){
                if (minX > i) minX = i;
                if (minY > j) minY = j;

                if (maxX < i) maxX = i;
                if (maxY < j) maxY = j;
            }
        }
    }

    //Affichage :
    printf("     ");
    for (int i = minX ; i < maxX+1 ; i++){
        printf("%d     ", i);
    }
    printf("\n");
    for (int j = minY ; j < maxY+1 ; j++){
        printf("     ");
        for (int i = minX ; i < maxX+1 ; i++){
            if (G->tab[i][j].id != UND){
                printf("%c      ", G->tab[i][j].sides[0].type);
            }
            else{
                printf("       ");
            }
        }
        printf("\n");

        printf("%d ", j);
        for (int i = minX ; i < maxX+1 ; i++){
            if (G->tab[i][j].id != UND){
                printf("%c %c %c  ", G->tab[i][j].sides[3].type, G->tab[i][j].sides[4].type,G->tab[i][j].sides[1].type);
            }
            else{
                // printf("         ");
                printf("       ");
            }
        }
        printf("\n");
        printf("     ");
        for (int i = minX ; i < maxX+1 ; i++){
            if (G->tab[i][j].id != UND){
                printf("%c      ", G->tab[i][j].sides[2].type);
            }
            else{
                printf("       ");
            }
        }
        printf("\n");

    }

}