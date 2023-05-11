#include "player.h"
#include "tile.h"
#include "ai.h"

#ifndef GAME_H
#define GAME_H

#define CLEAR "\033[2J\033[H"
#define SLEEPTIME 3.0

typedef struct game_t{
    int nb_players;
    stack* deck;
    grid* board;
    player** players;

}game;


void deal_tiles(game *G);

int is_in(int *tab, int value);
void append_visited_tiles(int value);
void print_visited_tiles();

int get_meeples_player(game *G, int meeple);
void give_points_to_max(game *G, int meeples[6], int points);
void remove_meeples_of_area(game *G, int meeples[6], int x, int y, int s);
    
int points_count(game* Game, int x, int y, int s, int start, int meeples[6], int final_evaluation);
int is_abbey_closed(game* G, int x, int y);

int print_ranking(game *G);

game* init_game(char* filename, int nb_players);

game* start_game(char* filename);
void gameplay(game* G);
void free_game(game* G);


game* start_creative_game();
void creative_gameplay(game* GC);
void free_creative_game(game *GC);

#endif