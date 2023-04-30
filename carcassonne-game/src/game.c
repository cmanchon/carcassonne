#include "../include/game.h"

game* init_game(char* filename, int nb_players){
	game* G = (game*)malloc(sizeof(game));
	G->nb_players = nb_players;
	G->board = init_grid();
	G->deck = get_tiles_from_file(filename);
	G->players = (player**)malloc(nb_players * sizeof(player*));
	
	for (int i = 0 ; i < nb_players ; i++){
		G->players[i] = init_player(i+1, UND);
	} 

	return G;
}


void free_game(game *G){
	if (G == NULL) return;
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


int is_area_closed(game* Game, int x, int y, int s, int start, int meeples[6]){
	//checks if in area is closed, returns the number of points if yes and -1 otherwise. gives the points to the player that has the biggest number of meeples on area
	//meeples contient le nombre d'occurence de chaque meeple, meeple[i] étant le meeple de couleur i (par defaut = {0, 0, 0, 0, 0, 0})
	//fonction recursive
	//copié collé de is_meeple_on_area donc attention 
	//faudrait gérer les blasons..... donc if G.tab machin == type marche plus
			//quoique si en fait pcq sur une tuile il y a que des blasons pas de ville ou l'inverse
	//BREF en tout cas faut compter les points
	//il faudra enlever les meeples quand une zone est completee aussi, a faire en dehors de la fonction

	// printf("visiting (%d ; %d)\t start = %d\n\n", x, y, start);
	if (start){
		for (int i = 0 ; i < VSIZE ; i++){
			visited_tiles[i] = UND;
		}
	} 

	if (Game->board->tab[x][y].id == UND) return -1;


	if (Game->board->tab[x][y].sides[s].meeple != UND){
			meeples[Game->board->tab[x][y].sides[s].meeple]++;
	}

	// print_visited_tiles();
	char type = Game->board->tab[x][y].sides[s].type;
	int nb_points = 0; 
	if (type == 'b') nb_points+=2;
	// conditions d'arrêt
	//      abbaye
	if (s == 4 && Game->board->tab[x][y].sides[0].type != type && Game->board->tab[x][y].sides[1].type != type && Game->board->tab[x][y].sides[2].type != type && Game->board->tab[x][y].sides[3].type != type && type == 'a'){      
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
		if (nb_points == 9) return nb_points;   // retourner que si c'est elle est complétée
		else return -1;
	}
	if (s != 4){
		//on compte les points de cette tuile :
		if (Game->board->tab[x][y].sides[4].type == type){
			// nb_points ++;       //pour side 4
			for (int i = 0 ; i < 4 ; i++){
				if (Game->board->tab[x][y].sides[i].type == type){
					int new_x = x, new_y = y;
					if (i == 0) new_y--;
					if (i == 1) new_x++;
					if (i == 2) new_y++;
					if (i == 3) new_x--;
					if (new_x >= NB_OF_TILES*2-1 || new_x <= 0 || new_y >= NB_OF_TILES*2-1 || new_y <= 0) return -1;    //on sort de la grille
					else if (Game->board->tab[new_x][new_y].id == UND) return -1;     //tuile adjacente vide 
					else if (i != s){      //tuile adjacente non vide et existante
						printf("tmping if\t\t\t%d points ; s=%d ; x = %d ; y = %d ; i = %d\n", nb_points, s, new_x, new_y, i);
						print_visited_tiles();
						if (!is_in(visited_tiles, Game->board->tab[new_x][new_y].id)){
							append_visited_tiles(Game->board->tab[new_x][new_y].id);
							int tmp = is_area_closed(Game, new_x, new_y, adjacent_side(i), 0, meeples);         //+1 pour current side
							printf("if tmp = %d ;  s=%d ; x = %d ; y = %d, i = %d\t%d points\n", tmp, s, new_x, new_y, i, nb_points);
							if (tmp != -1){
								if (type == 'c' || type == 'b'){
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
		else{
			int new_x = x, new_y = y;
			if (s == 0) new_y--;
			if (s == 1) new_x++;
			if (s == 2) new_y++;
			if (s == 3) new_x--;
			// if (s != 4 && Game->board->tab[x][y].sides[4].type != type){
			if (new_x >= NB_OF_TILES*2-1 || new_x <= 0 || new_y >= NB_OF_TILES*2-1 || new_y <= 0) return -1;    //on sort de la grille
			else if (Game->board->tab[new_x][new_y].id == UND) return -1;     //tuile adjacente vide 
			else if (start == 0){
				if (type == 'b' || type == 'c') return nb_points+2;
				else return nb_points+1;
			} 
			else {      //tuile adjacente non vide et existante
				printf("tmping else\t\t\t%d points ; s=%d ; x = %d ; y = %d\n", nb_points, s, new_x, new_y);
				print_visited_tiles();
				if (!is_in(visited_tiles, Game->board->tab[new_x][new_y].id)){
					append_visited_tiles(Game->board->tab[new_x][new_y].id);
					print_visited_tiles();
					int tmp = is_area_closed(Game, new_x, new_y, adjacent_side(s), 0, meeples);         //+1 pour current side
					printf("else tmp = %d ;  s=%d ; x = %d ; y = %d\t%d points\n", tmp, s, new_x, new_y, nb_points);
					if (tmp != -1) 
						if (type == 'c' || type == 'b'){
							nb_points+=tmp+2;
						}
						else{
							nb_points+=tmp+1;
						}
					else {
						return-1;
					}
					// return nb_points;           //jsp si il faut return mtn

				}
			}   
			// } 

		}
	}

	
	printf("return %d\n", nb_points);
	if (nb_points == 0 || (nb_points == 2 && type=='b') || type == 'p') return -1;
	return nb_points;

}

int get_meeples_player(game *G, int meeple){
	for (int i = 0 ; i < G->nb_players ; i++){
		if (G->players[i]->meeple_color == meeple) return i;
	}
	return UND;
}


game* give_points_to_max(game *G, int meeples[6], int points){
	//get the players who deserve the points      
		//on doit utiliser un tableau plutôt qu'une variable car il peut y avoir plusieurs joueurs qui ont un meeple
	if (points <= 0) return  G;
	int *player_max = (int*)malloc(G->nb_players*sizeof(int));
	memset(player_max, UND, G->nb_players*sizeof(int));
	int max = meeples[0];
	for (int i = 1 ; i < 6 ; i++){
		if (meeples[i] > max){
			memset(player_max, UND, G->nb_players*sizeof(int));
			player_max[0] = get_meeples_player(G, i);
			max = meeples[i];
		}
		else if (meeples[i] > 0 && meeples[i] == max){
			int j = 0;
			while (player_max[j] != UND) j++;
			player_max[j] = get_meeples_player(G, i);
		}
	}

	//give points to player_max
	int i = 0;
	printf("players_max = %d ; %d pour %d points\n", player_max[0], player_max[1], points);
	while (player_max[i] != UND){
		G->players[player_max[i]]->score += points;
		i++;
	} 
	print_player(G->players[0]);
	print_player(G->players[1]);
	free(player_max);
	return G;
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
		char choice;
		printf("\nDo you want to add AIs?\nNote: you can't add more than %d.\n", 6-tmp);
		
		nb_ai = UND;
		while (nb_ai < 0 || nb_ai > 6-tmp){
			scanf(" %c", &choice);

			if (choice == 'Y'){
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
			else if (atoi(&choice) > 0)
				nb_ai = atoi(&choice);
			else 
				nb_ai = 0;
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
			printf("%s 0 %s 1 %s 2 %s 3 %s 4 %s 5 %s\n", YELLOW, RED, GREEN, BLUE, BLACK, WHITE, END_FORMAT);
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


	if (nb_ai > 0) 
		AI_choose_meeple(G, nb_ai);


	rotate_tile(&G->deck->tab[G->deck->nb_tiles-1], 270);
	tile *T = pop(G->deck);
	place_tile_on_grid(G->board, T, 72, 72, UND);
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
	sleep(3);

	return G;
}




void gameplay(game *G){

	if (G == NULL){
		return;
	}

	int show_meeples = 0;
	while (G->board->nb_tiles < (NB_OF_TILES*2-1) * (NB_OF_TILES*2-1)){
		for (int i = 0 ; i < G->nb_players ; i++){
			if (G->players[i]->hand->nb_tiles == 0){
				printf(CLEAR);
				print_player(G->players[i]);
				printf("\n");
				print_grid(G->board, show_meeples, !show_meeples);
				printf("\n\nYou've placed all your tiles. Skiping to next player.\n");
				sleep(3);

				continue;
			} 
			if (G->players[i]->id > 6){			//AI
				AI_place_tile(G, i);
				continue;
			}
			tile * T = pop(G->players[i]->hand);
			char tmp = ' ';
			int j = 0;

			while((tmp != 'D' || tmp != 'G'|| tmp != 'Y'|| tmp != 'L'|| tmp != 'R') && j<10){
				printf(CLEAR);
				print_player(G->players[i]);
				printf("\n");
				print_grid(G->board, show_meeples, !show_meeples);
				printf("\n\nTile drawn: \n");
				print_tile(T, 0, 1);   
				printf("\nPress:\n");
				printf("\t%sR%s/%sD%s to rotate right\n", BOLD, END_FORMAT, BOLD, END_FORMAT);
				printf("\t%sL%s/%sG%s to rotate left\n", BOLD, END_FORMAT, BOLD, END_FORMAT);
				printf("\t%sM%s to switch board type.\n", BOLD, END_FORMAT);
				printf("\t%sP%s to draw again if tile cannot be placed\n", BOLD, END_FORMAT);
				printf("\t%sY%s to validate\n", BOLD, END_FORMAT);
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
					place_at_base_of_stack(G->players[i]->hand, T);
					T = pop(G->players[i]->hand);
					tmp = ' ';
				}
				else if (tmp == 'Q'){
					free_tile(T);
					free_game(G);
					exit(1);
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

			//placement
			int x = UND, y = UND;
			int buf = 0;
			int l = 0;
			while (buf == 0 || x == UND || y == UND){
				printf(CLEAR);
				print_player(G->players[i]);
				printf("\n");
				print_grid(G->board, 0, 1);
				printf("\n\nYour tile:\n");
				print_tile(T, 0, 1);
				printf("\n\nWrite where you want to place the tile (x ; y):\n");
				scanf("%d %d", &x, &y);
				buf = place_tile_on_grid(G->board, T, x, y, i);                    
			
				if (l >= 5){
					printf("Too many tries. Exiting.\n\n");
					free_tile(T);
					free_game(G);
					exit(1);
				}
				l++;
			}

			// debug
			// int t[6] = {0};
			// printf("\nis_area_closed(G->board, x, y, \033[1;37m0\033[0m, 1) = \033[1;37m%d\033[0m\n\n", is_area_closed(G, x, y, 0, 1, t));
			// memset(t, 0, 6*sizeof(int));
			// printf("\nis_area_closed(G->board, x, y, \033[1;37m1\033[0m, 1) = \033[1;37m%d\033[0m\n\n", is_area_closed(G, x, y, 1, 1, t));
			// memset(t, 0, 6*sizeof(int));
			// printf("\nis_area_closed(G->board, x, y, \033[1;37m2\033[0m, 1) = \033[1;37m%d\033[0m\n\n", is_area_closed(G, x, y, 2, 1, t));
			// memset(t, 0, 6*sizeof(int));
			// printf("\nis_area_closed(G->board, x, y, \033[1;37m3\033[0m, 1) = \033[1;37m%d\033[0m\n\n", is_area_closed(G, x, y, 3, 1, t));
			// memset(t, 0, 6*sizeof(int));
			// printf("\nis_area_closed(G->board, x, y, \033[1;37m4\033[0m, 1) = \033[1;37m%d\033[0m\n\n", is_area_closed(G, x, y, 4, 1, t));

			//meeple
			printf(CLEAR);
			print_player(G->players[i]);
			printf("\n");
			print_grid(G->board, 0, 1);
			sleep(1);

			char tmpm = ' ';
			show_meeples = 0;
			printf("You can switch the bord type by pressing %sM%s.\n\n", BOLD, END_FORMAT);
			while (tmpm != 'Y' && tmpm != 'N' && tmpm != 'Q'){
				printf("Place a meeple? (Y/N) ");
				scanf(" %c", &tmpm);

				if (tmpm == 'M'){
					show_meeples = !show_meeples;

					printf(CLEAR);
					print_player(G->players[i]);
					printf("\n");
					print_grid(G->board, show_meeples, !show_meeples);
					sleep(1);

				}
			}
			if (tmpm == 'Y'){
				int tmps = UND;
				buf = 0;
				while (buf == 0){
					printf(CLEAR);
					print_player(G->players[i]);
					printf("\n");
					print_grid(G->board, 1, 0);
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
						int t[6] = {0};
						int nb_points = is_area_closed(G, x, y, tmps, 1, t);
						if (nb_points > 0){
							G = give_points_to_max(G, t, nb_points);
						}
					}
				}
				printf(CLEAR);
				print_player(G->players[i]);
				printf("\n");
				print_grid(G->board, 1, 0);
				sleep(2);
			}
			else if (tmpm == 'Q'){
				free_tile(T);
				free_game(G);
				exit(1);
			}


			free_tile(T);
		}
	}


	free_game(G);
	return;   
}




//CREATIVE MODE
//utilisé principalement pour le debug, on utilise donc l'ancien affichage.


game* start_creative_game(){
	game* G = (game*)malloc(sizeof(game));
	G->nb_players = 1;
	G->players = (player**)malloc(G->nb_players * sizeof(player*));
	G->players[0] = init_player(1, 0);
	G->board = init_grid();
	G->deck = init_stack();



	return G;
}
void free_creative_game(game *GC){
	free_player(GC->players[0]);
	free(GC->players);
	free_grid(GC->board);
	free_stack(GC->deck);
	free(GC);
}

void creative_gameplay(game* GC){
	char tmp = ' ';
	int id_compteur = 0;
	while (tmp != 'Q'){
		printf("\n\nBOARD:\n\n");
		print_grid(GC->board, 0, 1);
		printf("\nTile disposition:\n");
		printf("\t  0\n");
		printf("\t3 4 1\n");
		printf("\t  2\n\n");
		tile * T = init_tile(' ', ' ', ' ', ' ', ' ', id_compteur);
		id_compteur++;
		int buf = 0;
		while (buf == 0){
			for (int i = 0 ; i < 5 ; i++){
				printf("\nside %d : ", i);
				scanf(" %c", &tmp);
				if (tmp == 'Q'){
					free_tile(T);
					free_creative_game(GC);
					exit(1);
				}
				T->sides[i].type = tmp;
			}
			print_tile(T, 0, 1);
			printf("Place this tile ? (Y/N)\n");
			scanf(" %c", &tmp);
			if (tmp == 'Y') buf = 1;
			else if (tmp == 'Q'){
				free_tile(T);
				free_creative_game(GC);
				exit(1);
			}
		}

		//placement
		int x = UND, y = UND;

		while (buf == 0 || x == UND || y == UND){
			printf("\n\nBOARD:\n\n");
			print_grid(GC->board, 0, 1);
			printf("\n");
			print_tile(T, 0, 1);
			printf("\n\nWrite where you want to place the tile:\n");
			scanf("%d %d", &x, &y);
			buf = place_tile_on_grid(GC->board, T, x, y, 1);
		}

		int t[6] = {0};
		printf("\nis_area_closed(GC->board, x, y, 0, 1) = %d\n\n", is_area_closed(GC, x, y, 0, 1, t));

		//meeple
		char tmpm = ' ';
		while (tmpm != 'Y' && tmpm != 'N' && tmpm != 'Q'){
			printf("Place a meeple? (Y/N) ");
			scanf(" %c", &tmpm);
		}
		int tmpa;
		if (tmpm == 'Y'){
			int tmps = UND;
			print_grid(GC->board, 1, 0);
			buf = 0;
			while (buf == 0){
				printf("choose your meeple color:\n");
				tmpa = UND;
				while (tmpa < 0 || tmpa >5){
					printf("\033[33m 0 \033[31m 1 \033[32m 2 \033[34m 3 \033[30m 4 \033[37m 5 \033[0m\n");
					scanf("%d", &tmpa);
				}
				GC->players[0]->meeple_color = tmpa;
				printf("\nWhich side?\n");
				printf("\t  0\n");
				printf("\t3 4 1\n");
				printf("\t  2\n\n");
				scanf("%d", &tmps);
				if (tmps < 0) break;        //if meeple cannot be placed: enter negative value

				// if (!is_meeple_on_area(GC->board, x, y, tmps)){
					printf("is meeple on area = %d\n", is_meeple_on_area(GC->board, x, y, tmps, 1));
					buf = place_meeple_on_tile(&GC->board->tab[x][y], tmps, GC->players[0]);
				// }
			}
			print_grid(GC->board, 1, 0);
		}
		else if (tmpm == 'Q'){
			free_tile(T);
			free_creative_game(GC);
			exit(1);
		}

		free_tile(T);
	}
}


