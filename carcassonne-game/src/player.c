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


int place_meeple_on_tile(tile* T, int side, player* P){
    //returns 0 if was unable to place a meeple
    if (T->sides[side].meeple != UND){
        return 0;
    } 
    
    T->sides[side].meeple = P->meeple_color;


    return 1;
}




int is_meeple_on_area(grid* G, int x, int y, int s){
    //checks if there's a meeple on the area of the side s of the tile at (x, y)
    //fonction récursive
    char type = G->tab[x][y].sides[s].type;

    // conditions d'arrêt
    int new_x = x, new_y = y;
    if (s == 0) new_y--;
    if (s == 1) new_x++;
    if (s == 2) new_y++;
    if (s == 3) new_x--;
    if (s!= 4 && G->tab[new_x][new_y].sides[adjacent_side(s)].meeple != UND) return 1;

    if (G->tab[x][y].sides[s].meeple != UND) return 1;
    if (G->tab[x][y].sides[4].type == type){
        if (G->tab[x][y].sides[4].meeple != UND) return 1;
        for (int i = 0 ; i < 4 ; i++){
            if (i != s){
                if (G->tab[x][y].sides[i].type == type){
                    if (G->tab[x][y].sides[4].meeple != UND) return 1;
                }
            }
        }
    }
    
                  
    for (int i = 0 ; i < 4 ; i++){
        if (G->tab[x][y].sides[i].type == type && G->tab[x][y].sides[4].type == type){
            if (G->tab[x][y].sides[i].meeple != UND) return 1;
            int adj_side = adjacent_side(i);
            int new_x = x, new_y = y;
            if (i == 0) new_y--;
            if (i == 1) new_x++;
            if (i == 2) new_y++;
            if (i == 3) new_x--;
            if (G->tab[new_x][new_y].id != UND){
                return is_meeple_on_area(G, new_x, new_y, adj_side);
            }
        }

    }
    
    
    return 0;
}


int is_area_closed(grid* G, int x, int y, int s){
    //checks if in area is closed, returns the number of points if yes and -1 otherwise. gives the points to the player that has the biggest number of meeples on area
    //fonction recursive
    //copié collé de is_meeple_on_area donc attention 
    char type = G->tab[x][y].sides[s].type;
    int nb_points = 0; 
    // conditions d'arrêt
    if (s == 4 && G->tab[x][y].sides[0].type != type && G->tab[x][y].sides[1].type != type && G->tab[x][y].sides[2].type != type && G->tab[x][y].sides[3].type != type ){      //abbaye
        if (y < NB_OF_TILES*2-1 && y > 0){
            if (x>0){
                if (G->tab[x-1][y-1].id != UND) nb_points++;
                if (G->tab[x-1][y].id != UND) nb_points++;
                if (G->tab[x-1][y+1].id != UND) nb_points++;
            }
            if (x < NB_OF_TILES*2-1){
                if (G->tab[x+1][y].id != UND) nb_points++;
                if (G->tab[x+1][y-1].id != UND) nb_points++;
                if (G->tab[x+1][y+1].id != UND) nb_points++;
            }
            if (G->tab[x][y+1].id != UND) nb_points++;
            if (G->tab[x][y-1].id != UND) nb_points++;
            if (G->tab[x][y].id != UND) nb_points++;

        }
    }


    //j'ai pas touché en dessous
    int new_x = x, new_y = y;
    if (s == 0) new_y--;
    if (s == 1) new_x++;
    if (s == 2) new_y++;
    if (s == 3) new_x--;
    if (s!= 4 && G->tab[new_x][new_y].sides[adjacent_side(s)].meeple != UND) return 1;

    if (G->tab[x][y].sides[s].meeple != UND) return 1;
    if (G->tab[x][y].sides[4].type == type){
        if (G->tab[x][y].sides[4].meeple != UND) return 1;
        for (int i = 0 ; i < 4 ; i++){
            if (i != s){
                if (G->tab[x][y].sides[i].type == type){
                    if (G->tab[x][y].sides[4].meeple != UND) return 1;
                }
            }
        }
    }
    
                  
    for (int i = 0 ; i < 4 ; i++){
        if (G->tab[x][y].sides[i].type == type && G->tab[x][y].sides[4].type == type){
            if (G->tab[x][y].sides[i].meeple != UND) return 1;
            int adj_side = adjacent_side(i);
            int new_x = x, new_y = y;
            if (i == 0) new_y--;
            if (i == 1) new_x++;
            if (i == 2) new_y++;
            if (i == 3) new_x--;
            if (G->tab[new_x][new_y].id != UND){
                return is_meeple_on_area(G, new_x, new_y, adj_side);
            }
        }

    }
    
    
    return 0;

}