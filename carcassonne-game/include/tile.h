#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#ifndef TUILE_H
#define TUILE_H

#define UND -1              //pour toutes les valeurs non definies, notamment à l'initialisation
#define NB_OF_TILES 72


#define BOLD "\033[1;37m"

#define CYAN "\033[36m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"

#define END_FORMAT "\033[0m"

typedef struct side_t{
	char type;
	int meeple;
}side;


typedef struct tile_t{
	int id;                 //if == UND (-1) -> vide
	int x, y;               //coordonnees sur la grille, initialisees a -1           
	side *sides;            //5 cotes
}tile;


typedef struct stack_t{
	tile* tab;
	int nb_tiles;
}stack;


typedef struct grid_t{
	tile** tab;
	int nb_tiles;
}grid;

//TILE
tile* init_tile(char side_A, char side_B, char side_C, char side_D, char side_E, int id);
void free_tile(tile* T);

void print_tile_info(tile* T);
void print_tile(tile *T, int show_meeples, int show_bg_colors);
void rotate_tile(tile* T, int degrees);
int adjacent_side(int s);
void adjacent_tile(int *x, int *y, int *s);
int typecmp(char t1, char t2);


//STACK
stack* init_stack();
void free_stack(stack *S);

tile* pop(stack *S);
void push(stack *S, tile *T);
tile* erase(stack *S, int ind);
void place_at_base_of_stack(stack* S, tile* T);
void copy_into(tile* Old, tile* New);
void print_stack(stack *S);
stack* get_tiles_from_file(char* filename);
void shuffle(stack* S);



//GRID
grid* init_grid();
void free_grid(grid* G);
int is_tile_placeable(grid* G, tile *T, int x, int y);
int place_tile_on_grid(grid* G, tile *T, int x, int y);
int is_meeple_on_area(grid* G, int x, int y, int s, int start);
void print_side(side S, int show_meeples, int show_bg_colors);
void print_grid(grid *G, int show_meeples, int show_bg_colors, int X_highlight, int Y_highlight);



#endif