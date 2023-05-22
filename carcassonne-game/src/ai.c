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




void AI_place_tile(game *G, int ind, int difficulty){
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



	srand((unsigned) time(NULL));
	if (difficulty == 0){
		//dumb IA
		int rng, proba = 7;
		int buf, l = 0;
		while (l < 49){

			for (i = minX ; i < maxX ; i++){
				for (j = minY ; j < maxY ; j++){
					buf = is_tile_placeable(G->board, T, i, j);
					if (buf){
						rng = rand() % 11;

						if (rng > proba){
							place_tile_on_grid(G->board, T, i, j);
							printf(CLEAR);
							print_player(G->players[ind]);
							printf("\n");
							print_grid(G->board, 0, 1);
							printf("\n\nPlayer %s%d%s (AI) placed a tile at (%d ; %d).\n", BOLD, G->players[ind]->id, END_FORMAT, i, j);
							sleep(5);
							free_tile(T);
							AI_place_meeple(G, ind, i, j, UND);
							return;
						
						}

					}
				}
			}


			rotate_tile(T, 90);
			l++;
			if (l % 4 == 0){	
				// si on a complètement retourné la tuile, on baisse la proba pour s'assurer de placer la tuile si possible
				proba = 0;
			}
			if (l % 8 == 0){
				// on considère que l'IA ne pouvait pas jouer sa tuile 
				//		-> elle en pioche une autre
				place_at_base_of_stack(G->players[ind]->hand, T);
				T = pop(G->players[i]->hand);
			}
		}
	}

	else if (difficulty > 0 && difficulty < 3){
		// - liste de coups possibles avec à chaque coup associé un score potentiel ( = 0 si la zone a déjà un ou des meeples différents de celui du joueur)
		// - tri de la liste en fonction du score probable
		// - choisit le coup le plus ou moins opti en fonction de la difficulté 

		possibility *possible_moves = (possibility*)malloc(0);
		int nb_moves = 0; 
		int potential_score = 0;
		int meeples[6] = {0};


		for (int degrees = 0 ; degrees < 360 ; degrees += 90){


			for (i = minX ; i < maxX ; i++){
				for (j = minY ; j < maxY ; j++){
					if (is_tile_placeable(G->board, T, i, j)){
						nb_moves++;
						possible_moves = realloc(possible_moves, nb_moves*sizeof(possibility));
						possible_moves[nb_moves-1].x = i;
						possible_moves[nb_moves-1].y = j;
						possible_moves[nb_moves-1].rotation = degrees;
						possible_moves[nb_moves-1].score = 0;
						possible_moves[nb_moves-1].opt_side = UND;
						
						
						// on compte le nombre de points potentiel
						for (int s = 0 ; s < 5 ; s++){
							memset(meeples, 0, 6*sizeof(int));
							potential_score = points_count(G, i, j, s, 1, meeples, 1);
							if (potential_score <= 0)
								continue;
							else if (potential_score >= possible_moves[nb_moves-1].score)
								possible_moves[nb_moves-1].opt_side = s;

							// on parcourt meeples pour vérifier qu'il n'y ait pas d'autres meeples dans la zone
							int other_meeple = 0, k = 0;
							while (!other_meeple && k < 6){
								if (k != G->players[ind]->meeple_color && meeples[k] > 0)
									other_meeple = 1;

								k++;
							}

							if (!other_meeple)
								possible_moves[nb_moves-1].score += potential_score;

						}
					}
				}
			}

			rotate_tile(T, 90);

			if (degrees >= 270 && nb_moves == 0){
				// la tuile ne peut pas être posée -> on en pioche une autre et on recommence la boucle
				place_at_base_of_stack(G->players[ind]->hand, T);
				T = pop(G->players[i]->hand);
				degrees = 0;
			}

		}

		//Debug
		for (int k = 0 ; k < nb_moves ; k ++){
			printf("%d : (%d;%d); ", possible_moves[k].score, possible_moves[k].x, possible_moves[k].y);
		}
		printf("\n\n");
		sleep(5);

		//tri de possible_moves en fonction du score
		int sorted = 0;
		do{
			sorted = 1;
			for (i = 0 ; i < nb_moves-1 ; i++){
				if (possible_moves[i+1].score <= possible_moves[i].score)
					continue;
				sorted = 0;
				possibility tmp = possible_moves[i+1];
				possible_moves[i+1] = possible_moves[i];
				possible_moves[i] = tmp;
			}
		} while(!sorted);


		//choix du move en fonction de difficulty
		//		1 = easy ; 2 = hard
		int rng;
		i = 0;
		while (1){
			rng = rand() % nb_moves+1;
			if (rng*difficulty >= nb_moves/2){

				rotate_tile(T, possible_moves[i].rotation);
				place_tile_on_grid(G->board, T, possible_moves[i].x, possible_moves[i].y);
				printf(CLEAR);
				print_player(G->players[ind]);
				printf("\n");
				print_grid(G->board, 0, 1);
				printf("\n\nPlayer %s%d%s (AI) placed a tile at (%d ; %d).\n", BOLD, G->players[ind]->id, END_FORMAT, possible_moves[i].x, possible_moves[i].y);
				sleep(5);
				free_tile(T);
				AI_place_meeple(G, ind, possible_moves[i].x, possible_moves[i].y, possible_moves[i].opt_side);
				break;
			}
			i++;

		}

		free(possible_moves);
	}
	else{
		printf("error AI_place_tile(): difficulty smaller than 0 or greater than 3");
		exit(1);
	}



}

void AI_place_meeple(game *G, int ind, int x, int y, int s){


	int meeple_placed = 0;

	if (s > UND){
		place_meeple_on_tile(&G->board->tab[x][y], s, G->players[ind]);
		meeple_placed = 1;
	}
	else{

		srand((unsigned) time(NULL));
		int proba = 6, rng;
		rng = rand() % 11; 			//l'IA a 4 chances sur 10 de poser un meeple
		if (G->board->tab[x][y].sides[4].type == 'a' && G->players[ind]->meeple_number > 0 && rng > proba-2){
			place_meeple_on_tile(&G->board->tab[x][y], 4, G->players[ind]);
			meeple_placed = 1;
		} 
		if (rng > proba && !meeple_placed && G->players[ind]->meeple_number > 0){
			int tmps;
			for (tmps = 0 ; tmps < 5 ; tmps++){
				
				rng = rand() % 11; 			//l'IA a 4 chances sur 10 de poser un meeple sur ce côté

				if (rng >= proba && G->board->tab[x][y].sides[tmps].type != 'p' && !meeple_placed){
					if (!is_meeple_on_area(G->board, x, y, tmps, 1)){
						place_meeple_on_tile(&G->board->tab[x][y], tmps, G->players[ind]);
						meeple_placed = 1;

						break;
					}

				}

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

	//évaluation
	int t[6] = {0};
	int nb_points;
	for (int j = 0 ; j < 4 ; j++){
		nb_points = points_count(G, x, y, j, 1, t, 0);
		if (nb_points > 0){
			give_points_to_max(G, t, nb_points);
			remove_meeples_of_area(G, t, x, y, j);
			sleep(10);

		}
		memset(t, 0, 6*sizeof(int));
	}

	return;
}