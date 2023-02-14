#include "tile.h"
#ifndef PLAYER_H
#define PLAYER_H

typedef struct player_t{
    int id;
    int meeple_color;
    int meeple_number;
    int score;

    stack* hand;
}player;


player* init_player(int numero, int meeple);
void free_player(player* P);

void print_player(player *P);

int place_meeple_on_tile(tile* T, int side, player* P);


#endif