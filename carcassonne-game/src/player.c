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

void print_players_hand(player *P){
    printf("Player %d\n", P->id);
    printf("%d tiles in hand\n", P->hand->nb_tiles);
    print_stack(P->hand);
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

int visited_tiles[VSIZE] = {UND};      

int is_in(int *tab, int value){
    //checks if value is in tab
    int i = 0;
    while (tab[i] != UND && i < VSIZE){
        if (tab[i] == value) return 1;
        i++;
    }
    return 0;
}

void append_visited_tiles(int value){
    int i = 0 ;
    while (visited_tiles[i] != UND && i < VSIZE){
        i++;
    }
    if (visited_tiles[i] == UND) visited_tiles[i] = value;
}

void print_visited_tiles(){
    int i = 0;
    printf("visited_tiles = (");
    while (visited_tiles[i] != UND && i < VSIZE){
        printf("%d ; ", visited_tiles[i]);
        i++;
    }
    printf(")\n");
}


int is_area_closed(grid* G, int x, int y, int s, int start){
    //checks if in area is closed, returns the number of points if yes and -1 otherwise. gives the points to the player that has the biggest number of meeples on area
    //fonction recursive
    //copié collé de is_meeple_on_area donc attention 
    //faudrait gérer les blasons..... donc if G.tab machin == type marche plus
            //quoique si en fait pcq sur une tuile il y a que des blasons pas de ville ou l'inverse
    //BREF en tout cas faut compter les points

    //on dirait que quand c'est des big structures ça compte 2 de plus ? donc ya moyen d'enlever 2 à la fin, et le mieux ce serait donc de faire *2 par exemple à la fin pour compter les points plutôt que au fur et à mesure
    printf("visiting (%d ; %d)\t start = %d\n\n", x, y, start);
    if (start){
        for (int i = 0 ; i < VSIZE ; i++){
            visited_tiles[i] = UND;
        }
    } 

    if (G->tab[x][y].id == UND) return -1;


    print_visited_tiles();
    char type = G->tab[x][y].sides[s].type;
    int nb_points = 0; 
    // conditions d'arrêt
    //      abbaye
    if (s == 4 && G->tab[x][y].sides[0].type != type && G->tab[x][y].sides[1].type != type && G->tab[x][y].sides[2].type != type && G->tab[x][y].sides[3].type != type && type == 'a'){      
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
        if (G->tab[x][y].sides[4].meeple != UND){
            //donner les les points au joueur qui a cette meeple color
        }
        return nb_points;   //en fait faudrait retourner que si c'est elle est complétée en soit....
    }
    if (s != 4){
        //on compte les points de cette tuile :
        if (G->tab[x][y].sides[4].type == type){
            // nb_points ++;       //pour side 4
            for (int i = 0 ; i < 4 ; i++){
                if (G->tab[x][y].sides[i].type == type){
                    int new_x = x, new_y = y;
                    if (i == 0) new_y--;
                    if (i == 1) new_x++;
                    if (i == 2) new_y++;
                    if (i == 3) new_x--;
                    if (new_x >= NB_OF_TILES*2-1 || new_x <= 0 || new_y >= NB_OF_TILES*2-1 || new_y <= 0) return -1;    //on sort de la grille
                    else if (G->tab[new_x][new_y].id == UND) return -1;     //tuile adjacente vide 
                    else if (i != s){      //tuile adjacente non vide et existante
                        printf("tmping if\t\t\t%d points ; s=%d ; x = %d ; y = %d ; i = %d\n", nb_points, s, new_x, new_y, i);
                        print_visited_tiles();
                        if (!is_in(visited_tiles, G->tab[new_x][new_y].id)){
                            append_visited_tiles(G->tab[new_x][new_y].id);
                            int tmp = is_area_closed(G, new_x, new_y, adjacent_side(i), 0);         //+1 pour current side
                            printf("if tmp = %d ;  s=%d ; x = %d ; y = %d, i = %d\t%d points\n", tmp, s, new_x, new_y, i, nb_points);
                            if (tmp != -1) nb_points+=tmp+1;
                            else {
                                return-1; 
                            }
                            // return nb_points;           //jsp si il faut return mtn
                        }
                    }   

                }
            } 

        }
        else{
            int new_x = x, new_y = y;
            if (s == 0) new_y--;
            if (s == 1) new_x++;
            if (s == 2) new_y++;
            if (s == 3) new_x--;
            // if (s != 4 && G->tab[x][y].sides[4].type != type){
            if (new_x >= NB_OF_TILES*2-1 || new_x <= 0 || new_y >= NB_OF_TILES*2-1 || new_y <= 0) return -1;    //on sort de la grille
            else if (G->tab[new_x][new_y].id == UND) return -1;     //tuile adjacente vide 
            else if (start == 0){
                return nb_points+1;
            } 
            else {      //tuile adjacente non vide et existante
                printf("tmping else\t\t\t%d points ; s=%d ; x = %d ; y = %d\n", nb_points, s, new_x, new_y);
                print_visited_tiles();
                if (!is_in(visited_tiles, G->tab[new_x][new_y].id)){
                    append_visited_tiles(G->tab[new_x][new_y].id);
                    print_visited_tiles();
                    int tmp = is_area_closed(G, new_x, new_y, adjacent_side(s), 0);         //+1 pour current side
                    printf("else tmp = %d ;  s=%d ; x = %d ; y = %d\t%d points\n", tmp, s, new_x, new_y, nb_points);
                    if (tmp != -1) nb_points+=tmp+1;
                    else {
                        return-1;
                    }
                    // return nb_points;           //jsp si il faut return mtn

                }
            }   
            // } 

        }
    }

    
    if (nb_points == 0) return -1;
    printf("return %d\n", nb_points);
    return nb_points;

}