#include "../include/player.h"

player* init_player(int numero, int meeple){
	player *P = (player*)malloc(sizeof(player));

	P->id = numero;
	P->meeple_color = meeple;
	P->meeple_number = 7;
	P->score = 0;

	P->hand = init_stack();

	return P;
}


void free_player(player *P){
	free_stack(P->hand);
	free(P);
}


void print_player(player *P){
	printf("Player %s%d%s\nmeeples : ", BOLD, P->id, END_FORMAT);
	switch (P->meeple_color)
		{
		case 0:
			printf(YELLOW);
			break;
		
		case 1:
			printf(RED);
			break;
		
		case 2:
			printf(GREEN);
			break;
		
		case 3:
			printf(BLUE);
			break;
		
		case 4:
			printf(CYAN);
			break;
		
		case 5:
			printf(MAGENTA);
			break;
		
		default:
			printf("\033[37m");          //white
			break;
		}

	printf(" %d%s\n", P->meeple_number, END_FORMAT);

	printf("score : %d\n", P->score);
	printf("%d tiles in hand\n", P->hand->nb_tiles);

}

void print_players_hand(player *P){
	printf("Player %d\n", P->id);
	printf("%d tiles in hand\n", P->hand->nb_tiles);
	print_stack(P->hand);
}


int place_meeple_on_tile(tile* T, int side, player* P){
	//returns 0 if was unable to place a meeple
	if (T->sides[side].meeple != UND || P->meeple_number <= 0){
		return 0;
	} 
	
	T->sides[side].meeple = P->meeple_color;
	P->meeple_number--;


	return 1;
}




int is_meeple_on_area(grid* G, int x, int y, int s, int start){
	//checks if there's a meeple on the area of the side s of the tile at (x, y)
	//fonction récursive

	char type = G->tab[x][y].sides[s].type;

	// conditions d'arrêt
	if (G->tab[x][y].sides[4].type == 'a' && G->tab[x][y].sides[4].meeple == UND && s == 4) return 0;
	int new_x = x, new_y = y, adj_side = s;
	adjacent_tile(&new_x, &new_y, &adj_side);

	if (s!= 4 && G->tab[new_x][new_y].sides[adj_side].meeple != UND) return 1;


	// on check la tuile actuelle
	if (G->tab[x][y].sides[s].meeple != UND) return 1;
	if (typecmp(G->tab[x][y].sides[4].type, type)){
		if (G->tab[x][y].sides[4].meeple != UND) return 1;
		for (int i = 0 ; i < 4 ; i++){
			if (i != s){
				if (typecmp(G->tab[x][y].sides[i].type, type)){
					if (G->tab[x][y].sides[4].meeple != UND) return 1;
				}
			}
		}
	}
	
	if (s == 4){
		// on parcourt de tous les côtes qui correspondent
		// note : ce sera toujours la première récursion car on appelle ensuite que des côtés inférieurs à 4
		for (int i = 0 ; i < 4 ; i++){
			if (typecmp(G->tab[x][y].sides[i].type, type)){
				if (G->tab[x][y].sides[i].meeple != UND) return 1;
				adj_side = i;
				new_x = x;
				new_y = y;
				adjacent_tile(&new_x, &new_y, &adj_side);
				if (G->tab[new_x][new_y].id != UND){
					return is_meeple_on_area(G, new_x, new_y, adj_side, 0);
				}
			}

		}
	}
				  
	else{
		for (int i = 0 ; i < 4 ; i++){
			if (typecmp(G->tab[x][y].sides[i].type, type) && typecmp(G->tab[x][y].sides[4].type, type)){
				if (i == s && start == 0) 
					continue;
				if (G->tab[x][y].sides[i].meeple != UND) return 1;
				adj_side = i;
				new_x = x;
				new_y = y;
				adjacent_tile(&new_x, &new_y, &adj_side);
				if (G->tab[new_x][new_y].id != UND){
					return is_meeple_on_area(G, new_x, new_y, adj_side, 0);
				}
			}

		}
		if (!typecmp(G->tab[x][y].sides[4].type, type) && start){
			adj_side = s;
			new_x = x;
			new_y = y;
			adjacent_tile(&new_x, &new_y, &adj_side);
			if (G->tab[new_x][new_y].id != UND){
				return is_meeple_on_area(G, new_x, new_y, adj_side, 0);
			}
		}
		
	}
	
	return 0;
}
