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

