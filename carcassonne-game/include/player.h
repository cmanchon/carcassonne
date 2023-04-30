#include "tile.h"
#ifndef PLAYER_H
#define PLAYER_H

#define VSIZE 50                //size of *visited_tiles

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
void print_players_hand(player *P);

int place_meeple_on_tile(tile* T, int side, player* P);

int is_meeple_on_area(grid* G, int x, int y, int s, int start);


#endif