#include "../include/game.h"

game* init_game(char* filename, int nb_players){
    game* G = (game*)malloc(sizeof(game));
    G->nb_players = nb_players;
    G->board = init_grid();
    G->deck = get_tiles_from_file(filename);
    G->players = (player**)malloc(nb_players * sizeof(player*));
    
    for (int i = 0 ; i < nb_players ; i++){
        G->players[i] = init_player(i, UND);
    } 

    return G;
}


void free_game(game *G){
    for (int i = 0 ; i < G->nb_players ; i++){
        free_player(G->players[i]);
    }
    free(G->players);
    free_stack(G->deck);
    free(G->board);
    free(G);
}