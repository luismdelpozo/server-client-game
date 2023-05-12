#ifndef AUX_FUNC_H
#define AUX_FUNC_H
#include <stdio.h> 
#include "../config.h"
#include "../GAME/player.h"

// Estructura que define los emparejamientos
typedef struct emparejamientos{
    int p1;
    int p2;
} PARED;

// Estructura para definir la cola 
struct NODE_FIFO{
    int socket;
    struct NODE_FIFO *next_socket;
};

int fifo_is_empty(); // Check if the queu is empty
void insert(int sock); // Insert new socket in the queue
int get_next_socket(); // Get next socket of the queue
void free_fifo(); // Free all the queue
int send_new_player(PLAYER p, int cmd_s); // Send new player to the client that have just connected
int send_std_msg(int cmd, int x, int y, int socket, int turn); // Send std messages to the other player connected

#endif