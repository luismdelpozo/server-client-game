#ifndef SHIP_H
#define SHIP_H

#include "../config.h"

/*enum STATES{
    DEAD,
    ALIVE
};*/

typedef struct POSITION_STR{
    int x;
    int y;
} POSITION;

typedef struct SHIP_STR{
    char name [MAX_SHIP_NAME];
    int lifes;
    int size;
    POSITION pos[PLANE];
    int dead;
} SHIP;

SHIP create_ship(int lifes, int size, char *name);
SHIP set_position(SHIP s, POSITION p0, POSITION p1);
SHIP attack(SHIP s, POSITION pos);

int min(int x, int y);
int max(int x, int y);

#endif
