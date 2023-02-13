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


void deal_cards(game *G){
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
    //shuffle
    deal_cards(G);

    return G;
}