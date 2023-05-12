#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Config definition
#define MEM_PATH_GAME "/home/dev/Desktop/WORKSPACE/MEMORY/gaming.bin" // Memory path where games are saved
#define PORT 8888 // Port used in the connection server-client
#define IP "127.0.0.1" // Ip used in the connection server-client
#define MAX_CLIENTS 2 // Max clients possibles connected to the server
#define TRUE   1 // True
#define FALSE  0 // False
#define YES_TURN 0 // Is your turn
#define NOT_TURN 1 // Is not your turn

// Game config
#define MAX_SHIPS 3 // Max ships per player
#define MAX_PLAYERS 2 // Max player connected
#define MAX_MAP_ROWS 10 // Max rows in the map
#define MAX_MAP_COLS 10 // Max cols in the map
#define MAX_SHIP_NAME 32 // Max characteres of the name of the ship
#define PLANE 2 // Plane of the map 2-D

// Message types
#define FINISH 0 // Finish command
#define LOSE 1 // Lose command
#define GOOD_ATACK 2 // Good attack command
#define BAD_ATTACK 3 // Bad atack command
#define WAIT_SERVER 4 // Wait server command
#define START_GAME 5 // Start game command
#define WAIT_PLAYER 6 // Wait player command
#define NEW_PLAYER 7 // New player command
#define ATACKING 8 // Attack player command

struct msg_new_player{
    int cmd;
    int id;
    int map_positions[MAX_MAP_ROWS][MAX_MAP_COLS];
};

struct msg_std{
    int cmd;
    int turn;
    int x, y;
};

#endif
