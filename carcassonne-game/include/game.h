#include "player.h"
#include "tile.h"

#ifndef GAME_H
#define GAME_H

typedef struct game_t{
    int nb_players;
    stack* deck;
    grid* board;
    player** players;

}game;


game* init_game(char* filename, int nb_players);
void free_game(game* G);
game* start_game(char* filename);
void deal_tiles(game *G);
void gameplay(game* G);
#endif