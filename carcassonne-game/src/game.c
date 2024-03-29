#include "../include/game.h"

game* init_game(char* filename, int nb_players){
	game* G = (game*)malloc(sizeof(game));
	G->nb_players = nb_players;
	G->board = init_grid();
	G->deck = get_tiles_from_file(filename);
	G->players = (player**)malloc(nb_players * sizeof(player*));
	G->difficulty = DFLT_DIFFICULTY;
	
	for (int i = 0 ; i < nb_players ; i++){
		G->players[i] = init_player(i+1, UND);
	} 

	return G;
}


void free_game(game *G){
	if (G == NULL) return;

	if (G->board->nb_tiles < NB_OF_TILES && G->deck->nb_tiles < NB_OF_TILES){
		//si la partie n'a pas été terminée -> on affiche le classement
		for (int i = 0 ; i < G->nb_players ; i++){
			if (G->players[i]->score > 0){
				printf(CLEAR);
				int winner = ranking(G);
				printf("\n\nPlayer %s%d%s was in the lead with %d points.\n\n", BOLD, G->players[winner]->id, END_FORMAT, G->players[winner]->score);
				break;
			}
		}
	
	}

	for (int i = 0 ; i < G->nb_players ; i++){
		free_player(G->players[i]);
	}
	free(G->players);
	free_stack(G->deck);
	free_grid(G->board);
	free(G);
}


void deal_tiles(game *G){
	//deals all the tiles from G.deck to G.players. to use at the start of the game
	int i = 0;
	while (G->deck->nb_tiles > 0){
		push(G->players[i]->hand, pop(G->deck));
		i++;
		if (i == G->nb_players) i=0;
	}
}



int visited_tiles[VSIZE] = {UND};      

int is_in(int *tab, int value){
	//checks if value is in tab
	int i = 0;
	while (tab[i] != UND && i < VSIZE){
		if (tab[i] == value) return 1;
		i++;
	}
	return 0;
}

void append_visited_tiles(int value){
	int i = 0 ;
	while (visited_tiles[i] != UND && i < VSIZE){
		i++;
	}
	if (visited_tiles[i] == UND) visited_tiles[i] = value;
}

void print_visited_tiles(){
	int i = 0;
	printf("visited_tiles = (");
	while (visited_tiles[i] != UND && i < VSIZE){
		printf("%d ; ", visited_tiles[i]);
		i++;
	}
	printf(")\n");
}


int points_count(game* Game, int x, int y, int s, int start, int meeples[6], int final_evaluation){
	//checks if in area is closed
	//if !final_evalutation -> returns the number of points if closed and -1 otherwise.
	//if final evaluation 	-> always returns the number of points
	//meeples contient le nombre d'occurence de chaque meeple, meeple[i] étant le meeple de couleur i (par defaut = {0, 0, 0, 0, 0, 0})
	//fonction recursive

	if (Game->board->tab[x][y].id == UND || Game->board->tab[x][y].sides[s].type == 'p') return -1;

	if (start){
		for (int i = 0 ; i < VSIZE ; i++){
			visited_tiles[i] = UND;
		}
	} 
	


	if (Game->board->tab[x][y].sides[s].meeple != UND){
			meeples[Game->board->tab[x][y].sides[s].meeple]++;
	}

	char type = Game->board->tab[x][y].sides[s].type;
	int nb_points = 0, tmp; 
	if (type == 'b'){
		if (final_evaluation)
			nb_points++;
		else
			nb_points+=2;
	}

	//      abbaye ou village
	if (s == 4 && (type == 'a' || type =='v') && !typecmp(Game->board->tab[x][y].sides[0].type,type) && !typecmp(Game->board->tab[x][y].sides[1].type, type) && !typecmp(Game->board->tab[x][y].sides[2].type, type) && !typecmp(Game->board->tab[x][y].sides[3].type, type)){      
		if (y < NB_OF_TILES*2-1 && y > 0){
			if (x>0){
				if (Game->board->tab[x-1][y-1].id != UND) nb_points++;
				if (Game->board->tab[x-1][y].id != UND) nb_points++;
				if (Game->board->tab[x-1][y+1].id != UND) nb_points++;
			}
			if (x < NB_OF_TILES*2-1){
				if (Game->board->tab[x+1][y].id != UND) nb_points++;
				if (Game->board->tab[x+1][y-1].id != UND) nb_points++;
				if (Game->board->tab[x+1][y+1].id != UND) nb_points++;
			}
			if (Game->board->tab[x][y+1].id != UND) nb_points++;
			if (Game->board->tab[x][y-1].id != UND) nb_points++;
			if (Game->board->tab[x][y].id != UND) nb_points++;

		}
		if (Game->board->tab[x][y].sides[4].meeple != UND){
			meeples[Game->board->tab[x][y].sides[4].meeple]++;
		}
		if (nb_points == 9 || final_evaluation) return nb_points;   
		else return -1;
	}
	if (s != 4){
		if (typecmp(Game->board->tab[x][y].sides[4].type, type)){
			if (Game->board->tab[x][y].sides[4].meeple != UND){
				meeples[Game->board->tab[x][y].sides[4].meeple]++;
			}	

			for (int i = 0 ; i < 4 ; i++){
				if (typecmp(type, Game->board->tab[x][y].sides[i].type)){
					if (Game->board->tab[x][y].sides[i].meeple != UND){
						meeples[Game->board->tab[x][y].sides[i].meeple]++;
					}

					int new_x = x, new_y = y, new_s = i;
					adjacent_tile(&new_x, &new_y, &new_s);
					if (new_x >= NB_OF_TILES*2-1 || new_x <= 0 || new_y >= NB_OF_TILES*2-1 || new_y <= 0){ //on sort de la grille -> zone ouverte
						if (final_evaluation)
							return nb_points+1;
						else
							return -1;   
					}   
					else if (Game->board->tab[new_x][new_y].id == UND){ //tuile adjacente vide -> zone ouverte
						if (final_evaluation)
							return nb_points+1;
						else
							return -1;   
					}
					else if (i != s){      //tuile adjacente non vide et existante
						if (!is_in(visited_tiles, Game->board->tab[new_x][new_y].id)){
							append_visited_tiles(Game->board->tab[new_x][new_y].id);
							tmp = points_count(Game, new_x, new_y, new_s, 0, meeples, final_evaluation);
							if (final_evaluation && tmp == -1){
								nb_points+=1;
							}

							else if (tmp != -1){
								if ((type == 'c' || type == 'b') && !final_evaluation){
									nb_points+=tmp+2;
								}
								else{
									nb_points+=tmp+1;
								}
							}
							else {
								return-1;
							}
							// return nb_points;           //jsp si il faut return mtn
						}
					}   

				}
			} 

		}
		else{ //[4].type != type
			int new_x = x, new_y = y, new_s = s;
			adjacent_tile(&new_x, &new_y, &new_s);
			if (new_x >= NB_OF_TILES*2-1 || new_x <= 0 || new_y >= NB_OF_TILES*2-1 || new_y <= 0){ //on sort de la grille -> zone ouverte
				if (final_evaluation)
					return nb_points+1;
				else
					return -1;    
			}
			else if (Game->board->tab[new_x][new_y].id == UND){ //tuile adjacente vide -> zone ouverte
				if (final_evaluation)
					return nb_points+1;
				else
					return -1;   
			} 
			else if (start == 0){
				if (Game->board->tab[new_x][new_y].sides[new_s].meeple != UND){
					meeples[Game->board->tab[new_x][new_y].sides[new_s].meeple]++;
				}
				if ((type == 'b' || type == 'c') && !final_evaluation) return nb_points+2;
				else return nb_points+1;
			} 
			else {      //tuile adjacente non vide et existante
				if (!is_in(visited_tiles, Game->board->tab[new_x][new_y].id)){
					append_visited_tiles(Game->board->tab[new_x][new_y].id);
					tmp = points_count(Game, new_x, new_y, new_s, 0, meeples, final_evaluation);
					if (final_evaluation && tmp == -1){
						nb_points+=1;
					}
					
					else if (tmp != -1) {
						if ((type == 'c' || type == 'b') && !final_evaluation){
							nb_points+=tmp+2;
						}
						else{
							nb_points+=tmp+1;

						}
					}
					else {
						return-1;
					}
					// return nb_points;           //jsp si il faut return mtn

				}
			}   

		}
	}
	else{	//s == 4
		for (int i = 0 ; i < 4 ; i++){
			if (typecmp(type, Game->board->tab[x][y].sides[i].type)){
				tmp = points_count(Game, x, y, i, 0, meeples, final_evaluation);
				if (tmp != -1 || final_evaluation) 
					if ((type == 'c' || type == 'b') && !final_evaluation){
						nb_points+=tmp+2;
					}
					else{
						nb_points+=tmp+1;
					}
				else {
					return-1;
				}
			}
		}
	}

	
	if (nb_points == 0 || (nb_points == 2 && type=='b' && !final_evaluation) || (nb_points == 1 && type=='b' && final_evaluation) || type == 'p') return -1;
	return nb_points;

}


int get_meeples_player(game *G, int meeple){
	for (int i = 0 ; i < G->nb_players ; i++){
		// if (G->players[i]->meeple_color == meeple) return i;
		if (G->players[i]->meeple_color == meeple){
			return i;
		}
	}
	return UND;
}


int is_abbey_closed(game* G, int x, int y){
	// checks if the tile at (x;y) is an abbey and if it's closed, or if the tile at (x;y) closes an abbey or a village
	// if so, gives 9 points to the according player

	int nb_points = 0;

	if (G->board->tab[x][y].sides[4].type == 'a' || G->board->tab[x][y].sides[4].type == 'v'){
		if (G->board->tab[x][y].sides[4].meeple == UND)
			return 0;
		if (y < NB_OF_TILES*2-1 && y > 0){
			if (x>0){
				if (G->board->tab[x-1][y-1].id != UND) nb_points++;
				if (G->board->tab[x-1][y].id != UND) nb_points++;
				if (G->board->tab[x-1][y+1].id != UND) nb_points++;
			}
			if (x < NB_OF_TILES*2-1){
				if (G->board->tab[x+1][y].id != UND) nb_points++;
				if (G->board->tab[x+1][y-1].id != UND) nb_points++;
				if (G->board->tab[x+1][y+1].id != UND) nb_points++;
			}
			if (G->board->tab[x][y+1].id != UND) nb_points++;
			if (G->board->tab[x][y-1].id != UND) nb_points++;
			if (G->board->tab[x][y].id != UND) nb_points++;

		}
		if (nb_points == 9){
			int player_ind = get_meeples_player(G, G->board->tab[x][y].sides[4].meeple);
			G->players[player_ind]->score += 9;
			G->players[player_ind]->meeple_number ++;
			G->board->tab[x][y].sides[4].meeple = UND;

			if (G->board->tab[x][y].sides[4].type == 'a')
				printf("\nAn abbey was completed, 9 points were given to\n\tPlayer %s%d%s\n", BOLD, G->players[player_ind]->id, END_FORMAT);
			else if (G->board->tab[x][y].sides[4].type == 'v')
				printf("\nA village was completed, 9 points were given to\n\tPlayer %s%d%s\n", BOLD, G->players[player_ind]->id, END_FORMAT);
			sleep(SLEEPTIME);
			return 1;
		}
	}

	else if (y < NB_OF_TILES*2-1 && y > 0){
		if (x>0){
			if (G->board->tab[x-1][y-1].sides[4].type == 'a' || G->board->tab[x-1][y-1].sides[4].type == 'v') 
				return is_abbey_closed(G, x-1, y-1);
			if (G->board->tab[x-1][y].sides[4].type == 'a' || G->board->tab[x-1][y].sides[4].type == 'v') 
				return is_abbey_closed(G, x-1, y);
			if (G->board->tab[x-1][y+1].sides[4].type == 'a' || G->board->tab[x-1][y+1].sides[4].type == 'v') 
				return is_abbey_closed(G, x-1, y+1);
		}
		if (x < NB_OF_TILES*2-1){
			if (G->board->tab[x+1][y].sides[4].type == 'a' || G->board->tab[x+1][y].sides[4].type == 'v') 
				return is_abbey_closed(G, x+1, y);
			if (G->board->tab[x+1][y-1].sides[4].type == 'a' || G->board->tab[x+1][y-1].sides[4].type == 'v') 
				return is_abbey_closed(G, x+1, y-1);
			if (G->board->tab[x+1][y+1].sides[4].type == 'a' || G->board->tab[x+1][y+1].sides[4].type == 'v') 
				return is_abbey_closed(G, x+1, y+1);
		}
		if (G->board->tab[x][y+1].sides[4].type == 'a' || G->board->tab[x][y+1].sides[4].type == 'v') 
			return is_abbey_closed(G, x, y+1);
		if (G->board->tab[x][y-1].sides[4].type == 'a' || G->board->tab[x][y-1].sides[4].type == 'v') 
			return is_abbey_closed(G, x, y-1);

	}
	
	return 0;
}



void give_points_to_max(game *G, int meeples[6], int points){
	//get the players who deserve the points      
		//on doit utiliser un tableau plutôt qu'une variable car il peut y avoir plusieurs joueurs qui ont un meeple
	if (points <= 0) return  ;
	int *player_max = (int*)malloc(G->nb_players*sizeof(int));
	memset(player_max, UND, G->nb_players*sizeof(int));
	int max = meeples[0];
	int i, j;
	for (i = 1 ; i < 6 ; i++){
		if (meeples[i] > max){
			memset(player_max, UND, G->nb_players*sizeof(int));
			player_max[0] = get_meeples_player(G, i);
			max = meeples[i];
		}
		else if (meeples[i] > 0 && meeples[i] == max){
			j = 0;
			while (player_max[j] != UND) j++;
			player_max[j] = get_meeples_player(G, i);
		}
	}


	//give points to player_max
	if (player_max[0] != UND && G->board->nb_tiles < NB_OF_TILES){		//on n'affiche pas si c'est la fin de la partie
		i = 0;
		int ind;
		printf("\nAn area was completed, %d points were given to\n", points);
		while (player_max[i] != UND){
			ind = player_max[i];
			G->players[ind]->score += points;
			usleep(500000);
			printf("\tPlayer %s%d%s\n", BOLD, G->players[ind]->id, END_FORMAT);
			i++;
		} 	
		sleep(SLEEPTIME);
	}
	free(player_max);
	return ;
}


void remove_meeples_of_area(game *G, int meeples[6], int x, int y, int s){
	
	char type = G->board->tab[x][y].sides[s].type;
	int empty[6] = {0};
	int i;

	if (type == 'a' || type == 'v'){
		meeples[G->board->tab[x][y].sides[4].meeple]--;
		G->board->tab[x][y].sides[4].meeple = UND;
		return;
	}

	int X = x, Y = y, S = s;
	int visited_tiles[VSIZE] = {UND};
	if (s == 4){
		for (i = 0 ; i < 4 ; i++){
			if (typecmp(type, G->board->tab[x][y].sides[i].type)){
				S = i;
				adjacent_tile(&X, &Y, &S);
				break;
			}
		}
	}

	int j = 0;
	int is_in_visited_tiles = 0;
	while (memcmp(meeples, empty, 6*sizeof(int)) != 0){

		//on verifie si la tuile courante a des meeples
		if (typecmp(type, G->board->tab[X][Y].sides[4].type)){
			for (i = 0 ; i < 5 ; i++){
				if (typecmp(type, G->board->tab[X][Y].sides[i].type) && G->board->tab[X][Y].sides[i].meeple !=UND){
					meeples[G->board->tab[X][Y].sides[i].meeple]--;
					int ind = get_meeples_player(G, G->board->tab[X][Y].sides[i].meeple);
					G->players[ind]->meeple_number++;
					G->board->tab[X][Y].sides[i].meeple = UND;
				}
			}
		}
		else{
			if (G->board->tab[X][Y].sides[S].meeple != UND){
				meeples[G->board->tab[X][Y].sides[S].meeple]--;
				int ind = get_meeples_player(G, G->board->tab[X][Y].sides[S].meeple);
				G->players[ind]->meeple_number++;
				G->board->tab[X][Y].sides[S].meeple = UND;
			}
		}

		j = 0;
		while (visited_tiles[j] != UND && visited_tiles[j] != 0 && j < VSIZE) j++;
		visited_tiles[j] = G->board->tab[X][Y].id;


		// on définit les nouveaux X, Y et S pour le parcours
		if (typecmp(type, G->board->tab[X][Y].sides[4].type)){
			for (i = 0 ; i < 4 ; i++){
				if (typecmp(type, G->board->tab[X][Y].sides[i].type)){
					S = i;
					adjacent_tile(&X, &Y, &S);

					// on vérifie que la tuile existe
					if (G->board->tab[X][Y].id == UND){
						if (i==3) 
							return;
						else
							continue;
					}

					// on break que si la tuile n'est pas dans visited_tiles
					j = 0;
					is_in_visited_tiles = 0;
					while (visited_tiles[j] != UND && visited_tiles[j] != 0 && j < VSIZE){
						if (visited_tiles[j] == G->board->tab[X][Y].id)
							is_in_visited_tiles = 1;
						j++;
					}
					if (!is_in_visited_tiles)
						break;
				}


				if (i == 3) return;
			}
		}
		else{
			adjacent_tile(&X, &Y, &S);
			// on vérifie que la tuile existe
			if (G->board->tab[X][Y].id == UND)
				return;

			// si dans visited_tiles -> on return
			j = 0;
			is_in_visited_tiles = 0;
			while (visited_tiles[j] != UND && visited_tiles[j] != 0 && j < VSIZE){
				if (visited_tiles[j] == G->board->tab[X][Y].id)
					return;
				j++;
			}
		}


	}

	return;

}


int ranking(game *G){
	//returns the index of the winner 

	int i;
	int *players_ind = (int*)malloc(G->nb_players * sizeof(int));
	for (i = 0 ; i < G->nb_players ; i++){
		players_ind[i] = i;
	}

	int sorted = 0, tmp;

	// tri 
	do {
		sorted = 1;
		for (i = 0 ; i < G->nb_players-1 ; i++){
			if (G->players[players_ind[i+1]]->score <= G->players[players_ind[i]]->score)
				continue;
			sorted = 0;
			tmp = players_ind[i+1];
			players_ind[i+1] = players_ind[i];
			players_ind[i] = tmp;
		}
	} while (!sorted);

	printf("RANKING\n\n");
	for (i = 0 ; i < G->nb_players ; i++){
		usleep(500000);
		printf("#%d: Player %s%d%s\n", i+1, BOLD, G->players[players_ind[i]]->id, END_FORMAT);
		printf("\t%d points\n\n", G->players[players_ind[i]]->score);
	}

	sleep(SLEEPTIME);

	int winner = players_ind[0];
	free(players_ind);


	return winner;
}




//GAME

game* start_game(char* filename){
	printf("########## CARCASSONNE ##########\n\n");
	printf("INFO: You can leave the game at any time by pressing %sQ%s.\n\n\n", BOLD, END_FORMAT);
	usleep(500000);
	int tmp=UND, l = 0, i;				// on utilise la variable l dans les boucles while pour éviter qu'elles bouclent à l'infini
	printf("How many players are there? [1 ; 6]\n");
	while (tmp <= 0 || tmp > 6){
		scanf("%d", &tmp);
		l++;
		if (l >= 5){
			printf("Too many tries. Exiting.\n\n");
			return NULL;
		}
	}

	// si possible : on propose d'ajouter une ou plusieurs IA
	int nb_ai = 0;
	l = 0;
	if (tmp < 6){
		
		nb_ai = UND;

		printf("How many AIs do you want to add? [0 ; %d]\n", 6-tmp);
		while (nb_ai < 0 || nb_ai > 6-tmp){
			scanf("%d", &nb_ai);

			if (l >=5){
				printf("\nToo many tries. Keeping the number of AIs to 0.\n\n");
				nb_ai = 0;
				break;
			}
			l++;
		}
		
	}


	game *G = init_game(filename, tmp+nb_ai);

	// si il y a des IA, on modifie leurs id pour les reconnaitre par la suite -> on prend des ID supérieurs au nombre de joueurs possibles
	for (i = G->nb_players-nb_ai ; i < G->nb_players ; i++){
		G->players[i]->id = ID_AI + i - G->nb_players+nb_ai;
	}

	// les joueurs choisissent leur meeple
	for (i = 0 ; i < G->nb_players-nb_ai ; i++){
		printf("\n\nPlayer %s%d%s: \n\n", BOLD, G->players[i]->id, END_FORMAT);
		printf("Choose your meeple color:\n");
		tmp = UND;
		l = 0;
		while (tmp < 0 || tmp > 5){
			printf("%s 0 %s 1 %s 2 %s 3 %s 4 %s 5 %s\n", YELLOW, RED, GREEN, BLUE, CYAN, MAGENTA, END_FORMAT);
			scanf("%d", &tmp);

			// on vérifie que cette couleur de meeple n'a pas déjà été choisie
			if (tmp >= 0 && tmp <= 5){
				for (int j = 0 ; j < G->nb_players ; j++){
					if (G->players[j]->meeple_color == tmp){
						tmp = UND;
						break;
					}
				}
			}


			if (l >= 5){
				printf("Too many tries. Exiting.\n\n");
				free_game(G);
				return NULL;
			}
			l++;
		}
		G->players[i]->meeple_color = tmp;
	}

	if (nb_ai > 0) {
		AI_choose_meeple(G, nb_ai);
		printf("\nAI difficulty is set to %s2%s (hard) by default. Press %sY%s to change it.\n", BOLD, END_FORMAT, BOLD, END_FORMAT);
		char choice = ' ';
		getchar();
		choice = (char) getchar();
		if (choice == 'Y'){
			printf("\n0 = DUMB\n");
			printf("1 = EASY\n");
			printf("2 = HARD\n\n");
			printf("Choose your difficulty [0;2]:\n");
			tmp = UND; 
			l = 0;
			while ((tmp < 0 || tmp > 2) && l <= 5){
				scanf("%d", &tmp);
				l++;
			}
			if (tmp >= 0 && tmp <= 2){
				G->difficulty = tmp;
				printf("\nAI difficulty was set to %d.\n", G->difficulty);
				sleep(SLEEPTIME);
			}

		}

	}

	//tuile de départ
	rotate_tile(&G->deck->tab[G->deck->nb_tiles-1], 270);
	tile *T = pop(G->deck);
	place_tile_on_grid(G->board, T, 72, 72);
	free_tile(T);
	
	shuffle(G->deck);
	deal_tiles(G);

	usleep(500000);

	printf(CLEAR);
	printf("%s%d%s players: \n\n",BOLD, G->nb_players, END_FORMAT);

	for (i = 0 ; i < G->nb_players ; i++){
		usleep(500000);
		print_player(G->players[i]);
		printf("\n");
	}
	sleep(SLEEPTIME);

	return G;
}




void gameplay(game *G){

	if (G == NULL){
		return;
	}

	int show_meeples = 0;
	while (G->board->nb_tiles < NB_OF_TILES){
		for (int i = 0 ; i < G->nb_players ; i++){

			if (G->players[i]->hand->nb_tiles == 0){
				printf(CLEAR);
				print_player(G->players[i]);
				printf("\n");
				print_grid(G->board, show_meeples, !show_meeples, UND, UND);
				printf("\n\nYou've placed all your tiles. Skiping to next player.\n");
				sleep(SLEEPTIME);

				continue;
			} 
			if (G->players[i]->id > 6){			//AI
				AI_place_tile(G, i);
				continue;
			}
			tile * T = pop(G->players[i]->hand);
			char tmp = ' ';
			int j = 0;

			while((tmp != 'D' || tmp != 'G'|| tmp != 'Y'|| tmp != 'L'|| tmp != 'R' || tmp != 'P') && j<10){
				printf(CLEAR);
				print_player(G->players[i]);
				printf("\n");
				print_grid(G->board, show_meeples, !show_meeples, UND, UND);
				printf("\n\nTile drawn: \n");
				print_tile(T, 0, 1);   
				printf("\nPress:\n");
				printf("\t%sR%s to rotate right\n", BOLD, END_FORMAT);
				printf("\t%sL%s to rotate left\n", BOLD, END_FORMAT);
				printf("\t%sP%s to draw again if tile cannot be placed\n", BOLD, END_FORMAT);
				printf("\t%sY%s to validate\n", BOLD, END_FORMAT);
				printf("\n\t%sM%s to switch board type.\n", BOLD, END_FORMAT);
				printf("\t%sC%s to see the ranking.\n", BOLD, END_FORMAT);
				scanf(" %c", &tmp);
				printf("\n");

				if (tmp == 'Y'){
					break;
				} 
				else if (tmp == 'D' || tmp == 'R'){
					rotate_tile(T, 90);
					tmp = ' ';
				}
				else if (tmp == 'G'|| tmp == 'L'){
					rotate_tile(T, 270);
					tmp = ' ';
				}
				else if (tmp == 'M'){
					show_meeples = !show_meeples;
					tmp = ' ';
				}
				else if (tmp == 'P'){
					//on vérifie que la tuile ne peut pas être posée
					
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

					int placeable = 0;
					for (int degrees = 0 ; degrees < 360 ; degrees += 90){

						for (int x = minX ; x <= maxX ; x++){
							for (int y = minY ; y <= maxY ; y++){
								if (is_tile_placeable(G->board, T, x, y)){
									placeable = 1;

									//on sort des boucles
									x = maxX;
									y = maxY;
									rotate_tile(T, -degrees-90);
									degrees = 360;
								}
							}
						}
						rotate_tile(T, 90);
					}
					if (placeable){
						printf("\nThis tile can be placed.\n\n");
						sleep(SLEEPTIME);
						tmp = ' ';
					}
					else{
						place_at_base_of_stack(G->players[i]->hand, T);
						if (G->players[i]->hand->nb_tiles == 1){
							tmp = 'S';
							break;
						}
						T = pop(G->players[i]->hand);
						tmp = ' ';
					}
				}
				else if (tmp == 'Q'){
					free_tile(T);
					free_game(G);
					exit(1);
				}
				else if (tmp == 'C'){
					printf(CLEAR);
					ranking(G);
					getchar();
					getchar();
					tmp = ' ';
				}
				else{
					tmp = ' ';
				}
				j++;
				if (j >= 8){
					printf("Too many tries. Quit or proceed to placement? (Q/P)\n");
					scanf(" %c", &tmp);

					if (tmp == 'P')
						break;
					else{
						free_tile(T);
						free_game(G);
						exit(1);
					}
				}
			}

			if (tmp == 'S'){		//skipping turn
				printf("\n\nYour tile cannot be placed. Skipping to next player.\n");
				sleep(SLEEPTIME);
				continue;
			}

			//placement
			int x = UND, y = UND;
			int buf = 0;
			int l = 0;
			while (buf == 0 || x == UND || y == UND){
				printf(CLEAR);
				print_player(G->players[i]);
				printf("\n");
				print_grid(G->board, 0, 1, UND, UND);
				printf("\n\nYour tile:\n");
				print_tile(T, 0, 1);
				printf("\n\nWrite where you want to place the tile (x ; y):\n");
				scanf("%d %d", &x, &y);
				buf = place_tile_on_grid(G->board, T, x, y);                    
			
				if (l >= 5){
					printf("Too many tries. Exiting.\n\n");
					free_tile(T);
					free_game(G);
					exit(1);
				}
				l++;
			}


			//meeple
			printf(CLEAR);
			print_player(G->players[i]);
			printf("\n");
			print_grid(G->board, 0, 1, x, y);
			sleep(SLEEPTIME/3);

			char tmpm = ' ';
			show_meeples = 0;
			if (G->players[i]->meeple_number <= 0){
				printf("You have used all your meeples.\n\n");
				sleep(SLEEPTIME);
			}
			else{
				printf("You can switch the bord type by pressing %sM%s.\n\n", BOLD, END_FORMAT);
				while (tmpm != 'Y' && tmpm != 'N' && tmpm != 'Q'){
					printf("Place a meeple? (Y/N) ");
					scanf(" %c", &tmpm);

					if (tmpm == 'M'){
						show_meeples = !show_meeples;

						printf(CLEAR);
						print_player(G->players[i]);
						printf("\n");
						print_grid(G->board, show_meeples, !show_meeples, x, y);

					}
				}

				if (tmpm == 'Y'){
					int tmps = UND;
					buf = 0;
					while (buf == 0){
						printf(CLEAR);
						print_player(G->players[i]);
						printf("\n");
						print_grid(G->board, 1, 0, x, y);
						printf("\nEnter a negative value to cancel.\n");
						printf("\nYour tile is at (%d ; %d).\n\n", x, y);
						printf("Which side?\n");
						printf("\t  0\n");
						printf("\t3 4 1\n");
						printf("\t  2\n\n");
						scanf("%d", &tmps);
						if (tmps < 0) break;        //if meeple cannot be placed: enter negative value
						if (!is_meeple_on_area(G->board, x, y, tmps, 1)){
							buf = place_meeple_on_tile(&G->board->tab[x][y], tmps, G->players[i]);
						}
					}
					printf(CLEAR);
					print_player(G->players[i]);
					printf("\n");
					print_grid(G->board, 1, 0, x, y);
					sleep(SLEEPTIME / (2.0/3.0));
				}
				else if (tmpm == 'Q'){
					free_tile(T);
					free_game(G);
					exit(1);
				}
			}

			//évaluation
			int t[6] = {0};
			int nb_points;
			is_abbey_closed(G, x, y);
			for (j = 0 ; j < 4 ; j++){
				nb_points = points_count(G, x, y, j, 1, t, 0);
				if (nb_points > 0){
					give_points_to_max(G, t, nb_points);
					remove_meeples_of_area(G, t, x, y, j);
					sleep(SLEEPTIME*2);
				}
				memset(t, 0, 6*sizeof(int));
			}


			free_tile(T);
		}

	}

	// fin de partie

	printf(CLEAR);
	printf("All the tiles were played: end of game!\n\n");
	usleep(500000);
	printf("Last evaluation...");
	sleep(SLEEPTIME/1.5);
	
	// il faut parcourir la grille une fois de plus pour récupérer les points de chaque zone où il y a un ou des meeples, pour ensuite donner les points aux joueurs
	
	int t[6] = {0};
	int empty_t[6] = {0};			//sert seulement à la comparaison
	int nb_points, tile_count = 0;
	for (int i = 0 ; i < NB_OF_TILES*2-1 ; i++){
		for (int j = 0 ; j < NB_OF_TILES*2-1 ; j++){
			if (G->board->tab[i][j].id == UND)
				continue;

			for (int k = 0 ; k < 5 ; k++){
				nb_points = points_count(G, i, j, k, 1, t, 1);
				if (nb_points > 0 && memcmp(t, empty_t, 6*sizeof(int)) != 0){
					give_points_to_max(G, t, nb_points);
				}
				memset(t, 0, 6*sizeof(int));
			}

			tile_count++;
			if (tile_count >= NB_OF_TILES){
				i = NB_OF_TILES*2-1;
				j = NB_OF_TILES*2-1;
			}
				
		}
	}

	sleep(SLEEPTIME/(2.0/3.0));

	printf(CLEAR);
	int winner = ranking(G);

	sleep(SLEEPTIME/3.0);
	printf("\n\n\nCongrats to Player %s%d%s who won with %d points!\n\n", BOLD, G->players[winner]->id, END_FORMAT, G->players[winner]->score);


	return;   
}




