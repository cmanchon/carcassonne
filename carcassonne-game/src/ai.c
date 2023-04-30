#include "../include/ai.h"

void AI_choose_meeple(game *G, int nb_ai){
	int i, j=0;

	int unused_meeples[6] = {0, 1, 2, 3, 4, 5};
	for (i = 0 ; i < G->nb_players-nb_ai ; i++){
		unused_meeples[G->players[i]->meeple_color] = UND;
	}

	for (i = G->nb_players-nb_ai ; i < G->nb_players ; i++){
		while (unused_meeples[j] == UND) j++;
		G->players[i]->meeple_color = j;
		unused_meeples[j] = UND;
	}
}




void AI_place_tile(game *G, int ind){
	// dumb AI for the moment

	tile * T = pop(G->players[ind]->hand);

	// on établit une fenêtre des tuiles placées pour ne pas avoir à parcourir tout le plateau
	int maxX = -1, maxY = -1;
	int minX = NB_OF_TILES*2, minY = NB_OF_TILES*2;
	int i, j;
	for (i = 0 ; i < NB_OF_TILES*2-1 ; i++){
		for (j = 0 ; j < NB_OF_TILES*2-1 ; j++){
			if (G->board->tab[i][j].id != UND){
				if (minX > i) minX = i;
				if (minY > j) minY = j;

				if (maxX < i) maxX = i;
				if (maxY < j) maxY = j;
			}
		}
	}
	if (minX > 0) minX--;
	if (minY > 0) minY--;
	if (maxX < NB_OF_TILES*2-1) maxX++;
	if (maxY < NB_OF_TILES*2-1) maxY++;



	int rng, proba = 7;
	srand((unsigned) time(NULL));
	int buf;
	for (int k = 0 ; k < 4 ; k++){

		for (i = minX ; i < maxX ; i++){
			for (j = minY ; j < maxY ; j++){
				buf = is_tile_placeable(G->board, T, i, j);
				if (buf){
					rng = rand() % 11;

					if (rng > proba){
						place_tile_on_grid(G->board, T, i, j, G->players[ind]->id);
						printf(CLEAR);
						print_player(G->players[ind]);
						printf("\n");
						print_grid(G->board, 0, 1);
						printf("\n\nPlayer %s%d%s (AI) placed a tile at (%d ; %d).\n", BOLD, G->players[ind]->id, END_FORMAT, i, j);
						sleep(5);
						free_tile(T);
						AI_place_meeple(G, ind, i, j);
						return;
					
					}

				}
			}
		}


		rotate_tile(T, 90);
	}

}

void AI_place_meeple(game *G, int ind, int x, int y){

	int meeple_placed = 0;

	if (G->board->tab[x][y].sides[4].type == 'a'){
		place_meeple_on_tile(&G->board->tab[x][y], 4, G->players[ind]);
		meeple_placed = 1;
	} 

	srand((unsigned) time(NULL));
	int proba = 7, rng;
	rng = rand() % 11; 			//l'IA a 2 chances sur 10 de poser un meeple
	if (rng <= proba && !meeple_placed) 
		return;


	int tmps;
	for (tmps = 0 ; tmps < 5 ; tmps++){
		
		rng = rand() % 11; 			//l'IA a 2 chances sur 10 de poser un meeple sur ce côté

		if (rng > proba && G->board->tab[x][y].sides[tmps].type != 'p'){
			if (!is_meeple_on_area(G->board, x, y, tmps, 1)){
				place_meeple_on_tile(&G->board->tab[x][y], tmps, G->players[ind]);
				int t[6] = {0};
				int nb_points = is_area_closed(G, x, y, tmps, 1, t);
				if (nb_points > 0){
					G = give_points_to_max(G, t, nb_points);
				}
				meeple_placed = 1;

				break;
			}

		}

	}

	if (meeple_placed){
		printf(CLEAR);
		print_player(G->players[ind]);
		printf("\n");
		print_grid(G->board, 1, 0);
		printf("\n\nPlayer %s%d%s (AI) placed a tile at (%d ; %d).\n", BOLD, G->players[ind]->id, END_FORMAT, x, y);
		sleep(2);
	}


	return;
}