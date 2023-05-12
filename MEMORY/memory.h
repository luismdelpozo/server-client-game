#ifndef MEMORY_H
#define MEMORY_H

#include "../GAME/game.h"
#include "../GAME/player.h"

void read_all_games(void);
int write_game(GAME g);
GAME read_game(int id);
int get_max_game_id();

#endif