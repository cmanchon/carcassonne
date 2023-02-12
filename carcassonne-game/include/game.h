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

#endif