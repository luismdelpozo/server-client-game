#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>
#include <time.h>
#include "../config.h"
#include "ship.h"
#include "map.h"

typedef struct PLAYER_STR{
    int id;
    SHIP my_ships[MAX_SHIPS];
    MAP my_map;
    MAP rival_map;
} PLAYER;

PLAYER create_player(int id);
PLAYER set_ships(PLAYER p1);
int pos_is_valid(int x, int y, int size, int ship_created[MAX_SHIPS][4]);
int distance_two_points(int x1, int y1, int x2, int y2);
int is_inside_two_points(int d1, int d2, int dt);
int player_deleted(PLAYER p1);

#endif
