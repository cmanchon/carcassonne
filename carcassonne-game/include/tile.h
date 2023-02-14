#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef TUILE_H
#define TUILE_H

#define UND -1              //pour toutes les valeurs non definies, notamment à l'initialisation
#define NB_OF_TILES 72


typedef struct side_t{
    char type;
    int meeple;
}side;


typedef struct tile_t{
    int id;                 //if == UND (-1) -> vide
    side *sides;            //5 cotes
    int state;              //posee ou non
    int played_by;          //id du joueur qui l'a posee
    int x, y;               //coordonnees sur la grille, initialisees a -1           
    int blason;         

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


//STACK
stack* init_stack();
void free_stack(stack *S);

tile* pop(stack *S);
void copy_into(tile* old, tile* new);
tile* erase(stack *S, int ind);
void push(stack *S, tile *T);
void print_stack(stack *S);
stack* get_tiles_from_file(char* filename);
void shuffle(stack* S);



//GRID
grid* init_grid();
void free_grid(grid* G);
int place_tile_on_grid(grid* G, tile *T, int x, int y, int player);
void print_side(side S, int show_meeples, int show_bg_colors);
void print_grid(grid *G, int show_meeples, int show_bg_colors);



#endif