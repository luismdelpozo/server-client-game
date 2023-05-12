#ifndef GAME_H
#define GAME_H

#include "../config.h"
#include "player.h"

typedef struct GAME_STR{
    int id;
    PLAYER my_player;
    PLAYER rival_player;
} GAME;

GAME create_game(int id, PLAYER p1, PLAYER p2);

#endif
