#include "ship.h"

SHIP create_ship(int lifes, int size, char *name){
    SHIP s;
    strcpy(s.name, name);
    s.lifes = lifes;
    s.size = size;
    s.dead = 0;
    
    POSITION pos;
    pos.x = 0;
    pos.y = 0;

    s.pos[0] = pos;
    s.pos[1] = pos;
    
    return s;
}

SHIP set_position(SHIP s, POSITION p0, POSITION p1){
    s.pos[0].x = p0.x;
    s.pos[0].y = p0.y;
    s.pos[1].x = p1.x;
    s.pos[1].y = p1.y;
    return s;
}

int max(int x, int y){
    if(x > y) return x;
    return y;
}

int min(int x, int y){
    if (x < y) return x;
    return y;
}