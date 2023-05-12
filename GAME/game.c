#include "game.h"

GAME create_game(int id, PLAYER p1, PLAYER p2){
    GAME g;
    g.id = id;
    g.my_player = p1;
    g.rival_player = p2;
    return g;
}
