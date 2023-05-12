#include "aux_func.h"
#include "../GAME/player.h"
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>     

struct NODE_FIFO *first = NULL; // First node in the queue
struct NODE_FIFO *last = NULL; // Last node in the queue

// Check if the queue is empty
int fifo_is_empty(){
    if (first == NULL) return 1;
    else return 0;
}

// Insert new node in the queue
void insert(int sock){
    struct NODE_FIFO *new_node; // Pointer to the new queue
    new_node = malloc(sizeof(struct NODE_FIFO)); // Alloc memory to the new node
    new_node->socket = sock;
    new_node->next_socket = NULL;
    if (fifo_is_empty()){
        first = new_node;
        last = new_node;
    }
    else{
        last->next_socket = new_node;
        last = new_node;
    }
}

// Get next node in the queue
int get_next_socket(){
    if (!fifo_is_empty()){
        int new_socket = first->socket;
        struct NODE_FIFO *del = first;
        if (first == last){
            first = NULL;
            last = NULL;
        }
        else{
            first = first->next_socket;
        }
        free(del);
        return new_socket;
    }
    else
        return -1;
}

// Method to free the queue
void free_fifo(){
    printf("Freeing the memory...\n");
    struct NODE_FIFO *reco = first;
    struct NODE_FIFO *bor;
    while (reco != NULL){
        bor = reco;
        reco = reco->next_socket;
        free(bor);
    }
}

int send_std_msg(int cmd, int x, int y, int socket, int turn){
    struct msg_std std_msg;

    std_msg.cmd = cmd;
    std_msg.turn = turn;
    std_msg.x = x;
    std_msg.y = y;

    char buf_data[sizeof(std_msg)];
    memcpy(buf_data, &std_msg, sizeof(std_msg));

    // Enviamos mensaje
    if(send(socket, &buf_data, sizeof(std_msg), 0) < 0){  
        perror("send create new player failed\n");
        return -1;  
    }

    return 0;
}

int send_new_player(PLAYER p, int cmd_s){
    struct msg_new_player new_msg; 

    new_msg.cmd = cmd_s;
    new_msg.id = p.id;
    for (int x = 0; x < MAX_MAP_ROWS; x++){
        for(int y = 0; y < MAX_MAP_COLS; y++){
            new_msg.map_positions[x][y] = p.my_map.positions[x][y]; 
        }
    }

    char buf_data[sizeof(new_msg)];
    memcpy(buf_data, &new_msg, sizeof(new_msg));
    //printf("msg: %s\n", buf_data);

    // Enviamos mensaje -> WAITPAYER ID and MAP POSITIONS
    if(send(p.id, &buf_data, sizeof(new_msg), 0) < 0){  
        perror("send create new player failed\n");
        return -1;  
    }

    return 0;
}
