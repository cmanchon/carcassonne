#include "include/game.h"


int main(int argc, char **argv){
	
	if (argc == 2 && !strcmp(argv[1], "clear"))
		printf(CLEAR);

	game *G = start_game("data/tuiles_base_simplifiees.csv");
	gameplay(G);
	free_game(G);

	return 0;
}

