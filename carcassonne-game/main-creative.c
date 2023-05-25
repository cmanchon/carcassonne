#include "include/game.h"


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
			buf = place_tile_on_grid(GC->board, T, x, y);
		}

		// int t[6] = {0};
		// printf("\nis_area_closed(GC->board, x, y, 0, 1) = %d\n\n", is_area_closed(GC, x, y, 0, 1, t));

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





int main(void){
    
    game *GC = start_creative_game();
    creative_gameplay(GC);
    free_creative_game(GC);
    

    return 0;
}