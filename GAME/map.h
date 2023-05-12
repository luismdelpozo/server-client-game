#ifndef MAP_H
#define MAP_H

#include "../config.h"

/*enum COLUMS{
    A,   
    B,   
    C,   
    D,   
    E,   
    F,   
    G,   
    H,  
    I,   
    J,
    K,   
    L
};*/

typedef struct MAP_STR{
    int id;
    int positions[MAX_MAP_COLS][MAX_MAP_ROWS];
} MAP;

MAP create_map(int id);
void print_map(MAP m);
MAP change_map(MAP m, int x, int y, int value);

#endif
