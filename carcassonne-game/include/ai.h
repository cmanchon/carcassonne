
#ifndef AI_H
#define AI_H
#include "game.h"

#define ID_AI 42

void AI_choose_meeple(game *G, int nb_ai);
void AI_place_tile(game *G, int ind);
void AI_place_meeple(game *G, int ind, int x, int y);

#endif