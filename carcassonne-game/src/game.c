#include "../include/game.h"

game* init_game(char* filename, int nb_players){
    game* G = (game*)malloc(sizeof(game));
    G->nb_players = nb_players;
    G->board = init_grid();
    G->deck = get_tiles_from_file(filename);
    G->players = (player**)malloc(nb_players * sizeof(player*));
    
    for (int i = 0 ; i < nb_players ; i++){
        G->players[i] = init_player(i+1, UND);
    } 

    return G;
}


void free_game(game *G){
    for (int i = 0 ; i < G->nb_players ; i++){
        free_player(G->players[i]);
    }
    free(G->players);
    free_stack(G->deck);
    free_grid(G->board);
    free(G);
}


void deal_tiles(game *G){
    //deals all the tiles from G.deck to G.players. to use at the start of the game
    int i = 0;
    while (G->deck->nb_tiles > 0){
        push(G->players[i]->hand, pop(G->deck));
        i++;
        if (i == G->nb_players) i=0;
    }
}

game* start_game(char* filename){
    //affichage plus sérieux à faire
    printf("welcome to carcassonne\n\n");
    int tmp=UND;
    while (tmp <= 0 || tmp >6){
        printf("how many players? \n");
        scanf("%d", &tmp);
    }
    game *G = init_game(filename, tmp);

    //........gérer les joueurs
    for (int i = 0 ; i < G->nb_players ; i++){
        printf("\n\nPlayer %d: \n\n", i+1);
        printf("choose your meeple color:\n");
        tmp = UND;
        while (tmp < 0 || tmp >5){
            printf("\033[33m 0 \033[31m 1 \033[32m 2 \033[34m 3 \033[30m 4 \033[37m 5 \033[0m\n");
            scanf("%d", &tmp);
        }
        G->players[i]->meeple_color = tmp;
    }

    rotate_tile(&G->deck->tab[G->deck->nb_tiles-1], 270);
    tile *T = pop(G->deck);
    place_tile_on_grid(G->board, T, 72, 72, UND);
    printf("\n\n\nstarting board:\n\n");
    print_grid(G->board, 0, 1);
    free_tile(T);
    shuffle(G->deck);
    deal_tiles(G);

    return G;
}




void gameplay(game *G){

    while (G->board->nb_tiles < (NB_OF_TILES*2-1) * (NB_OF_TILES*2-1)){
        for (int i = 0 ; i < G->nb_players ; i++){
            print_player(G->players[i]);
            printf("\n\nBOARD:\n\n");
            print_grid(G->board, 0, 1);
            tile * T = pop(G->players[i]->hand);
            printf("\n\ntile drawn : \n");
            char tmp = ' ';
            int j = 0;
            //rotate or not
            while((tmp != 'D' || tmp != 'G'|| tmp != 'Y'|| tmp != 'L'|| tmp != 'R') && j<10){
                print_tile(T, 0, 1);   
                printf("Press:\n");
                printf("\t\033[1;37mR\033[0m/\033[1;37mD\033[0m to rotate right\n");
                printf("\t\033[1;37mL\033[0m/\033[1;37mG\033[0m to rotate left\n");
                printf("\t\033[1;37mP\033[0m to draw again if tile cannot be placed\n");
                printf("\t\033[1;37mY\033[0m to validate\n");
                scanf(" %c", &tmp);
                printf("\n");
                if (tmp == 'Y'){
                    break;
                } 
                else if (tmp == 'D' || tmp == 'R'){
                    rotate_tile(T, 90);
                    tmp = ' ';
                }
                else if (tmp == 'G'|| tmp == 'L'){
                    rotate_tile(T, 270);
                    tmp = ' ';
                }
                else if (tmp == 'P'){
                    place_at_base_of_stack(G->players[i]->hand, T);
                    T = pop(G->players[i]->hand);
                    tmp = ' ';

                }
                else if (tmp == 'Q'){
                    free_tile(T);
                    free_game(G);
                    exit(1);
                }
                else{
                    tmp = ' ';
                }
                j++;
            }

            //placement
            int x = UND, y = UND;
            int buf = 0;

            while (buf == 0 || x == UND || y == UND){
                printf("\n\nBOARD:\n\n");
                print_grid(G->board, 0, 1);
                printf("\n");
                print_tile(T, 0, 1);
                printf("\n\nWrite where you want to place the tile:\n");
                scanf("%d %d", &x, &y);
                buf = place_tile_on_grid(G->board, T, x, y, i);                     //might be i+1 idk
            }

            print_grid(G->board, 0, 1);
            //meeple
            char tmpm = ' ';
            while (tmpm != 'Y' && tmpm != 'N' && tmpm != 'Q'){
                printf("Place a meeple? (Y/N) ");
                scanf(" %c", &tmpm);
            }
            if (tmpm == 'Y'){
                int tmps = UND;
                print_grid(G->board, 1, 0);
                buf = 0;
                while (buf == 0){
                    printf("\nWhich side?\n");
                    printf("\t  0\n");
                    printf("\t3 4 1\n");
                    printf("\t  2\n\n");
                    scanf("%d", &tmps);
                    if (!is_meeple_on_area(G->board, &G->board->tab[x][y], tmps)){
                        buf = place_meeple_on_tile(&G->board->tab[x][y], tmps, G->players[i]);          //might be i+1 idk
                    }
                }
                print_grid(G->board, 1, 0);
            }
            // else if (tmpm == 'N'){
                
            // }
            else if (tmpm == 'Q'){
                free_tile(T);
                free_game(G);
                exit(1);
            }


            free_tile(T);
        }
    }


    free_game(G);
    return;   
}