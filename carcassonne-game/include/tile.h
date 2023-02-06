#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef TUILE_H
#define TUILE_H

#define UND -1              //pour toutes les valeurs non definies, notamment à l'initialisation
#define NB_OF_TILES 72

typedef struct side_t{
    char type;
    int meeple;
}side;


typedef struct tile_t{
    int id;
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


tile* init_tile(char side_A, char side_B, char side_C, char side_D, char side_E, int id);
void free_tile(tile* T);

void print_tile_info(tile* T);

stack* init_stack();
void free_stack(stack *S);
tile* pop(stack *S);
void push(stack *S, tile T);
void print_stack(stack *S);
stack* get_tiles_from_file(char* filename);
stack* get_tiles_from_file2(char* filename);


grid* init_grid();
void free_grid(grid* G);


#endif