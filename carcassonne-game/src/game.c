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
	//affichage plus sérieux à faire
	printf("welcome to carcassonne\n\n");
	int tmp=UND;
	while (tmp <= 0 || tmp >6){
		printf("how many players? \n");
		scanf("%d", &tmp);
	}
	game *G = init_game(filename, tmp);

	//........gérer les joueurs
	for (int i = 0 ; i < G->nb_players ; i++){
		printf("\n\nPlayer %d: \n\n", i+1);
		printf("choose your meeple color:\n");
		tmp = UND;
		while (tmp < 0 || tmp >5){
			printf("\033[33m 0 \033[31m 1 \033[32m 2 \033[34m 3 \033[30m 4 \033[37m 5 \033[0m\n");
			scanf("%d", &tmp);
		}
		G->players[i]->meeple_color = tmp;
	}

	rotate_tile(&G->deck->tab[G->deck->nb_tiles-1], 270);
	tile *T = pop(G->deck);
	place_tile_on_grid(G->board, T, 72, 72, UND);
	printf("\n\n\nstarting board:\n\n");
	print_grid(G->board, 0, 1);
	free_tile(T);
	shuffle(G->deck);
	deal_tiles(G);

	return G;
}




void gameplay(game *G){

	while (G->board->nb_tiles < (NB_OF_TILES*2-1) * (NB_OF_TILES*2-1)){
		for (int i = 0 ; i < G->nb_players ; i++){
			if (G->players[i]->hand->nb_tiles == 0) continue;
			print_player(G->players[i]);
			printf("\n\nBOARD:\n\n");
			print_grid(G->board, 0, 1);
			tile * T = pop(G->players[i]->hand);
			printf("\n\ntile drawn : \n");
			char tmp = ' ';
			int j = 0;
			//rotate or not
			while((tmp != 'D' || tmp != 'G'|| tmp != 'Y'|| tmp != 'L'|| tmp != 'R') && j<10){
				print_tile(T, 0, 1);   
				printf("Press:\n");
				printf("\t\033[1;37mR\033[0m/\033[1;37mD\033[0m to rotate right\n");
				printf("\t\033[1;37mL\033[0m/\033[1;37mG\033[0m to rotate left\n");
				printf("\t\033[1;37mP\033[0m to draw again if tile cannot be placed\n");
				printf("\t\033[1;37mY\033[0m to validate\n");
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
			}

			//placement
			int x = UND, y = UND;
			int buf = 0;

			while (buf == 0 || x == UND || y == UND){
				printf("\n\nBOARD:\n\n");
				print_grid(G->board, 0, 1);
				printf("\n");
				print_tile(T, 0, 1);
				printf("\n\nWrite where you want to place the tile:\n");
				scanf("%d %d", &x, &y);
				buf = place_tile_on_grid(G->board, T, x, y, i);                     //might be i+1 idk
			}

			print_grid(G->board, 0, 1);
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
			char tmpm = ' ';
			while (tmpm != 'Y' && tmpm != 'N' && tmpm != 'Q'){
				printf("Place a meeple? (Y/N) ");
				scanf(" %c", &tmpm);
			}
			if (tmpm == 'Y'){
				int tmps = UND;
				print_grid(G->board, 1, 0);
				buf = 0;
				while (buf == 0){
					printf("\nWhich side?\n");
					printf("\t  0\n");
					printf("\t3 4 1\n");
					printf("\t  2\n\n");
					scanf("%d", &tmps);
					if (tmps < 0) break;        //if meeple cannot be placed: enter negative value
					if (!is_meeple_on_area(G->board, x, y, tmps)){
						buf = place_meeple_on_tile(&G->board->tab[x][y], tmps, G->players[i]);          //might be i+1 idk
						int t[6] = {0};
						int nb_points = is_area_closed(G, x, y, tmps, 1, t);
						printf("%d points\n", nb_points);
						if (nb_points > 0){
							G = give_points_to_max(G, t, nb_points);
							for (int k = 0 ; k < G->nb_players ; k++){
								print_player(G->players[k]);
							}

						}
					}
				}
				print_grid(G->board, 1, 0);
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
			buf = place_tile_on_grid(GC->board, T, x, y, 1);                     //might be i+1 idk
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
					printf("is meeple on area = %d\n", is_meeple_on_area(GC->board, x, y, tmps));
					buf = place_meeple_on_tile(&GC->board->tab[x][y], tmps, GC->players[0]);          //might be i+1 idk
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


