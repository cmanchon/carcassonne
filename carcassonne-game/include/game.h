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
void deal_tiles(game *G);

int is_in(int *tab, int value);
void append_visited_tiles(int value);
void print_visited_tiles();
int is_area_closed(game* G, int x, int y, int s, int start);

game* start_game(char* filename);
void gameplay(game* G);


game* start_creative_game();
void free_creative_game(game *GC);
void creative_gameplay(game* GC);

#endif