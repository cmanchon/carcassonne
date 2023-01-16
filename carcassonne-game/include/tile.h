#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef TUILE_H
#define TUILE_H

#define UND -1              //pour toutes les valeurs non definies, notamment à l'initialisation

typedef struct side_t{
    char* type;
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


tile* init_tile(char* side_A, char* side_B, char* side_C, char* side_D, char* side_E, int id);
void free_tile(tile* T);
void print_tile_info(tile* T);


#endif