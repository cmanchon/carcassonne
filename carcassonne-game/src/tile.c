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
    //for debugging
    int show_meeples = 0, show_bg_colors = 1;           
    // int show_meeples = 1, show_bg_colors = 0;
    printf("Tuile %d :\n", T->id);

    //affichage basique
    // printf("  %c  \n", T->sides[0].type);
    // printf("%c %c %c\n", T->sides[3].type, T->sides[4].type, T->sides[1].type);
    // printf("  %c  \n\n", T->sides[2].type);

    //affichage avec couleurs
    printf("   ");
    print_side(T->sides[0], show_meeples, show_bg_colors);
    printf("\n");
    print_side(T->sides[3], show_meeples, show_bg_colors);
    print_side(T->sides[4], show_meeples, show_bg_colors);
    print_side(T->sides[1], show_meeples, show_bg_colors);
    printf("\n");
    printf("   ");
    print_side(T->sides[2], show_meeples, show_bg_colors);
    printf("\n");
    

    printf("state : %d\n", T->state);
    printf("played_by : %d\n", T->played_by);
    printf("coords : (%d, %d)\n", T->x, T->y);
    printf("blason : %d\n", T->blason);
}


void print_tile(tile *T, int show_meeples, int show_bg_colors){
    // int show_meeples = 1, show_bg_colors = 0;
    printf("   ");
    print_side(T->sides[0], show_meeples, show_bg_colors);
    printf("\n");
    print_side(T->sides[3], show_meeples, show_bg_colors);
    print_side(T->sides[4], show_meeples, show_bg_colors);
    print_side(T->sides[1], show_meeples, show_bg_colors);
    printf("\n");
    printf("   ");
    print_side(T->sides[2], show_meeples, show_bg_colors);
    printf("\n");
    
}


void rotate_tile(tile* T, int degrees){
    int tmpi1, tmpi2;
    char tmpc1, tmpc2;
    if (degrees == 90 || degrees == -270){
        tmpi1 = T->sides[1].meeple;
        tmpc1 = T->sides[1].type;
        T->sides[1].meeple = T->sides[0].meeple;
        T->sides[1].type = T->sides[0].type;
        
        tmpi2 = T->sides[2].meeple;
        tmpc2 = T->sides[2].type;
        T->sides[2].meeple = tmpi1;
        T->sides[2].type = tmpc1;
        
        tmpi1 = T->sides[3].meeple;
        tmpc1 = T->sides[3].type;
        T->sides[3].meeple = tmpi2;
        T->sides[3].type = tmpc2;

        T->sides[0].meeple = tmpi1;
        T->sides[0].type = tmpc1;

    }
    else if (degrees == 180){
        rotate_tile(T, 90);
        rotate_tile(T, 90);
    }
    else if (degrees == 270 || degrees == -90){
        rotate_tile(T, 90);
        rotate_tile(T, 90);
        rotate_tile(T, 90);
    }
    else if (degrees == 0){
        return;
    }
    else{
        printf("error rotate_tile() : degrees value can't be divided by 90\n");
        exit(1);
    }
}

void copy_into(tile *old, tile* new){
    //copies old's attributes into new, admitting they've both already been allocated
    new->id = old->id;

    new->sides[0].meeple = old->sides[0].meeple;
    new->sides[0].type = old->sides[0].type;
    new->sides[1].meeple = old->sides[1].meeple;
    new->sides[1].type = old->sides[1].type;
    new->sides[2].meeple = old->sides[2].meeple;
    new->sides[2].type = old->sides[2].type;
    new->sides[3].meeple = old->sides[3].meeple;
    new->sides[3].type = old->sides[3].type;
    new->sides[4].meeple = old->sides[4].meeple;
    new->sides[4].type = old->sides[4].type;

    new->state = old->state;
    new->played_by = old->played_by;
    new->x = old->x;
    new->y = old->y;
    new->blason = old->blason;
}


int adjacent_side(int s){
    //returns the adjacent side of s on a grid
    if (s == 0) return 2;
    else if (s == 2) return 0;
    else if (s == 1) return 3;
    else if (s == 3) return 1;
    else return UND;
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

tile* pop(stack *S){s
    if (S->nb_tiles == 0){
        return NULL;
    }
    else{
        tile *L = &(S->tab[S->nb_tiles-1]);
        tile* L_copy = init_tile(L->sides[0].type, L->sides[1].type, L->sides[2].type, L->sides[3].type, L->sides[4].type, L->id);
        L = NULL;
        free(S->tab[S->nb_tiles-1].sides);
        S->tab = realloc(S->tab, (S->nb_tiles - 1) * sizeof(tile));
        S->nb_tiles--;
        return L_copy;
    }
}

void push(stack *S, tile *T) {
    S->tab = realloc(S->tab, (S->nb_tiles + 1) * ( sizeof(tile)+ 5*sizeof(side) ));
    
    S->tab[S->nb_tiles] = *T;
    S->tab[S->nb_tiles].sides = T->sides;
    
    S->nb_tiles++;
    free(T);
}

tile* erase(stack *S, int ind){
    //erases indth element of S
    //-> places the element to erase at the top of the stack to pop it next

    tile* T= init_tile(' ', ' ', ' ', ' ', ' ', UND);
    copy_into(&S->tab[ind], T);
    for (int i = ind ; i < S->nb_tiles-1 ; i++){
        copy_into(&S->tab[i+1], &S->tab[i]);
    }

    copy_into(T, &S->tab[S->nb_tiles-1]);                           //1 2 3 4 5

    free_tile(T);
    
    return pop(S);
}


void place_at_base_of_stack(stack* S, tile* T){
    //places T at the base of S, useful if a player needs to draw another tile
    tile* tmp= init_tile(' ', ' ', ' ', ' ', ' ', UND);
    push(S, tmp);

    for (int i = S->nb_tiles-1 ; i > 0 ; i--){
        copy_into(&S->tab[i-1], &S->tab[i]);
    }
    copy_into(T, &S->tab[0]);
    free_tile(T);

}




void shuffle(stack* E){
    stack* S = init_stack();
    int rng;
    srand((unsigned) time(NULL));
    // algo de melange
    while (E->nb_tiles > 0){
        rng = rand() % E->nb_tiles;
        push(S, erase(E, rng));
    }
    
    //copy S back into E
    while(S->nb_tiles > 0){
        push(E, pop(S));
    }

    free_stack(S);

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
    int n = NB_OF_TILES;
    // int n = 7;
    FILE *fh = fopen(filename, "r");

    if (fh == NULL){
        printf("get_tiles_from_file : erreur d'ouverture de %s\n", filename);
        exit(1);
    }
    stack* S = (stack*)malloc(sizeof(stack));
    S->tab = (tile*)malloc(n*sizeof(tile));
    for (int i = 0 ; i < n ; i++){
        S->tab[i].sides = (side*)malloc(5*sizeof(side));
    }
    S->nb_tiles = n;

    size_t size;
    char *buf = NULL;


    
    for (int j = 0 ; j < n ; j++){
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


int place_tile_on_grid(grid* G, tile *T, int x, int y, int player){
    //returns 1 si could be placed, 0 otherwise

    if (G->tab[x][y].id != UND){
        //there's already a tile at this place
        return 0;
    }
    else if (x <= 0 || y <= 0 || x >= NB_OF_TILES*2-1 || y>= NB_OF_TILES*2-1) return 0;
    else if (G->tab[x-1][y].id == UND && G->tab[x][y-1].id == UND && G->tab[x+1][y].id == UND && G->tab[x][y+1].id == UND && G->nb_tiles > 0) return 0;
    else if (G->tab[x-1][y].id != UND && x-1 >=0 && G->tab[x-1][y].sides[1].type != T->sides[3].type) return 0;
    else if (G->tab[x][y-1].id != UND && y-1 >=0 && G->tab[x][y-1].sides[2].type != T->sides[0].type) return 0;
    else if (G->tab[x+1][y].id != UND && x+1 <=NB_OF_TILES*2-1 && G->tab[x+1][y].sides[3].type != T->sides[1].type) return 0;
    else if (G->tab[x][y+1].id != UND && y+1 <= NB_OF_TILES*2-1 && G->tab[x][y+1].sides[0].type != T->sides[2].type) return 0;

    //tile can be placed
    //G->tab[x][y] = *T;
    //G->tab[x][y].sides = T.sides;
    
    G->tab[x][y].id = T->id;
    G->tab[x][y].blason = T->blason;
    G->tab[x][y].sides[0] = T->sides[0];
    G->tab[x][y].sides[1] = T->sides[1];
    G->tab[x][y].sides[2] = T->sides[2];
    G->tab[x][y].sides[3] = T->sides[3];
    G->tab[x][y].sides[4] = T->sides[4];

    G->tab[x][y].state = 1;
    G->tab[x][y].x = x;
    G->tab[x][y].y = y;
    G->tab[x][y].played_by = player;

    G->nb_tiles++;
    //free_tile(T);
    

    return 1;
}

void print_side(side S, int show_meeples, int show_bg_colors){
    if (show_bg_colors != 0){
        switch (S.type)
        {
        case 'b':
            printf("\033[45");       //purple
            break;
        
        case 'p':
            printf("\033[42");       //green
            break;
        
        case 'r':
            printf("\033[44");       //blue
            break;

        case 'c':
            printf("\033[43");       //yellow
            break;
        
        case 'v':
            printf("\033[41");       //red
            break;
        
        case 'a':
            printf("\033[46");       //cyan
            break;
        
        default:
            printf("\033[40");       //black
            break;
        }
        if (show_meeples==0) printf("m");
        else printf(";");
    }

    if (show_meeples!=0){
        if (show_bg_colors == 0) printf("\033[");
        switch (S.meeple)
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
    }

    printf(" %c \033[0m", S.type);
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

void print_grid(grid *G, int show_meeples, int show_bg_colors){
    //on parcourt une première fois pour établir la fenêtre d'où sont les tuiles placées 
    if (G->nb_tiles == 0){
        printf("grid empty\n");
        return;
    }

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
    printf("       ");
    printf("\033[1;37m");
    if (minX > 0) minX--;
    if (minY > 0) minY--;
    if (maxX < NB_OF_TILES*2-1) maxX++;
    if (maxY < NB_OF_TILES*2-1) maxY++;
    for (int i = minX ; i < maxX+1 ; i++){
        if (i >= 100) printf("%d      ", i);
        if (i < 100 && i >= 10) printf(" %d      ", i);
        if (i < 10) printf(" %d       ", i);
    }
    printf("\033[0m");
    printf("\n");
    for (int j = minY ; j < maxY+1 ; j++){
        printf("       ");
        for (int i = minX ; i < maxX+1 ; i++){
            if (G->tab[i][j].id != UND){
                // printf("%c       ", G->tab[i][j].sides[0].type);
                print_side(G->tab[i][j].sides[0], show_meeples, show_bg_colors);
                printf("      ");
            }
            else{
                printf("         ");
                // printf("        ");
            }
        }
        printf("\n");

        printf("\033[1;37m");
        if (j >= 100) printf("%d ", j);
        else if (j < 100 && j >= 10) printf(" %d ", j);
        else if (j < 10) printf("  %d ", j);
        printf("\033[0m");
        for (int i = minX ; i < maxX+1 ; i++){
            if (G->tab[i][j].id != UND){
                // printf("%c %c %c   ", G->tab[i][j].sides[3].type, G->tab[i][j].sides[4].type,G->tab[i][j].sides[1].type);
                print_side(G->tab[i][j].sides[3], show_meeples, show_bg_colors);
                // printf(" ");
                print_side(G->tab[i][j].sides[4], show_meeples, show_bg_colors);
                // printf(" ");
                print_side(G->tab[i][j].sides[1], show_meeples, show_bg_colors);
                // printf(" ");
            }
            else{
                printf("         ");
                // printf("        ");
            }
        }
        printf("\n");
        printf("       ");
        for (int i = minX ; i < maxX+1 ; i++){
            if (G->tab[i][j].id != UND){
                // printf("%c        ", G->tab[i][j].sides[2].type);
                print_side(G->tab[i][j].sides[2], show_meeples, show_bg_colors);
                printf("      ");
            }
            else{
                printf("         ");
                // printf("        ");
            }
        }
        printf("\n");

    }

}