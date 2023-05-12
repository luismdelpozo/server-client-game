#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
     
#include "../config.h"
#include "../GAME/game.h" 
#include "../MEMORY/memory.h"
#include "aux_func.h"

/*
    COMPILATION SERVER.C:
    
    gcc -Wall 
    ../config.h 
    ../GAME/ship.h     ../GAME/ship.c 
    ../GAME/map.h      ../GAME/map.c 
    ../GAME/player.h   ../GAME/player.c 
    ../GAME/game.h     ../GAME/game.c 
    ../MEMORY/memory.h ../MEMORY/memory.c 
    aux_func.h         aux_func.c
    server.c -o server -lm
*/
     
int main(int argc , char *argv[])  
{  
    int opt = TRUE;  
    int master_socket, n_bytes_recv, socket_to_sent, addrlen, new_socket, client_socket[MAX_CLIENTS], activity, i, sd, max_sd;  
    fd_set readfds;

    int id_game = get_max_game_id();

    // Create all the games
    GAME game_pared[MAX_CLIENTS/2];
    for(i = 1; i <= MAX_CLIENTS/2; i++){
        PLAYER p1 = create_player(0);
        PLAYER p2 = create_player(0);
        game_pared[i] = create_game(id_game + i, p1, p2);
    }

    PARED par[MAX_CLIENTS/2];
    for(i = 0; i < MAX_CLIENTS/2; i++){
        par->p1 = 0;
        par->p2 = 0;
    }

    struct sockaddr_in address;
    memset(&address, '\0', sizeof(address));      
      
    for (i = 0; i < MAX_CLIENTS; i++) {  
        client_socket[i] = 0;  
    }  
         
    //create a master socket 
    if((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0){  
        perror("socket creation failed\n");  
        exit(EXIT_FAILURE);  
    }  
     
    //set master socket to allow multiple connections
    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0){  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
     
    //type of socket created 
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons(PORT);  
         
    //bind the socket to localhost port 8888 
    if(bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0){  
        perror("bind function failed\n");  
        exit(EXIT_FAILURE);  
    }   
         
    //try to specify maximum of 3 pending connections for the master socket 
    if (listen(master_socket, MAX_CLIENTS) < 0){  
        perror("listen function failed\n");  
        exit(EXIT_FAILURE);  
    }  
         
    //accept the incoming connection 
    addrlen = sizeof(address);  
    puts("Waiting for connections ...");  

    int number_of_connections = 0;
    while(TRUE) {  
        FD_ZERO(&readfds);  
     
        //add master socket to set 
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
             
        for (i = 0; i < MAX_CLIENTS; i++){  
            sd = client_socket[i];  
                
            if(sd > 0)  
                FD_SET(sd, &readfds);  
 
            if(sd > max_sd)  
                max_sd = sd; 
        } 
     
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);   

        if (activity < 0){  
            perror("select function error\n");  
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(master_socket, &readfds)){   
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }

            PLAYER p = create_player(new_socket);
            p = set_ships(p);
            
            if (number_of_connections < MAX_CLIENTS){
                
                send_new_player(p, WAIT_PLAYER);

                for (i = 0; i < MAX_CLIENTS/2; i++){
                    if(par[i].p1 == 0){
                        game_pared[i].my_player = p;
                        par[i].p1 = new_socket;
                        break;
                    }
                    if(par[i].p2 == 0){
                        game_pared[i].rival_player = p;
                        par[i].p2 = new_socket;

                        send_std_msg(START_GAME, 0, 0, par[i].p1, YES_TURN);
                        send_std_msg(START_GAME, 0, 0, par[i].p2, NOT_TURN);

                        printf("Starting game with players %d and %d\n", par[i].p1, par[i].p2);
                        break;
                    }
                }

                //add new socket to array of sockets 
                for (i = 0; i < MAX_CLIENTS; i++){ 
                    if(client_socket[i] == 0){  
                        client_socket[i] = new_socket; 
                        break;
                    }
                    else{
                        FD_ZERO(&readfds);
                        FD_SET(client_socket[i], &readfds);
                    } 
                }

                number_of_connections++;
            }
            else{                                
                send_new_player(p, WAIT_SERVER);

                printf("Socket %d is waiting\n", new_socket);
                insert(new_socket);
            }                  
        }  

        //else its some IO operation on some other socket
        for (i = 0; i < MAX_CLIENTS; i++){  
            sd = client_socket[i];
            
            if (FD_ISSET(sd, &readfds) && par[i/2].p1 > 0 && par[i/2].p2 > 0){

                struct msg_std std_msg;
                char recv_data[sizeof(std_msg)];       
                n_bytes_recv = recv(sd, recv_data, sizeof(std_msg), 0);
                if(n_bytes_recv < 0){
                    perror("Recv std message failed\n");
                    exit(EXIT_FAILURE);
                }
                
                memcpy(&std_msg, recv_data, n_bytes_recv);

                if (sd == par[i/2].p2) socket_to_sent = par[i/2].p1;
                else socket_to_sent = par[i/2].p2;

                int x = 0;
                int y = 0;

                if (std_msg.cmd == FINISH || std_msg.cmd == LOSE){
                    printf("Message type %d sent by %d\n", std_msg.cmd, sd);
                    send_std_msg(std_msg.cmd, x, y, socket_to_sent, YES_TURN);
                    printf("Message type %d sent to %d\n", std_msg.cmd, socket_to_sent);

                    client_socket[i] = 0;
                    if (i%2 == 0) client_socket[i+1] = 0;
                    else client_socket[i-1] = 0;

                    write_game(game_pared[i/2]);
                    number_of_connections--;
                    number_of_connections--;
                    
                    close(par[i/2].p1);
                    close(par[i/2].p2);                          

                    game_pared[i/2].my_player.id = 0;
                    game_pared[i/2].rival_player.id = 0;
                    
                    par[i/2].p1 = 0;
                    par[i/2].p2 = 0;

                    while(number_of_connections < MAX_CLIENTS && fifo_is_empty() == 0){
                        int next_socket_fifo = get_next_socket();
                        number_of_connections++;

                        if (par[i/2].p1 == 0){
                            par[i/2].p1 = next_socket_fifo;
                            game_pared[i/2].my_player.id = next_socket_fifo;
                            
                            if (i%2 == 0) client_socket[i] = next_socket_fifo;
                            else client_socket[i-1] = next_socket_fifo;    
                        } 
                        else if(par[i/2].p2 == 0){
                            par[i/2].p2 = next_socket_fifo;
                            game_pared[i/2].rival_player.id = next_socket_fifo;

                            if (i%2 == 0) client_socket[i+1] = next_socket_fifo;
                            else client_socket[i] = next_socket_fifo;

                            send_std_msg(START_GAME, 0, 0, par[i].p1, YES_TURN);
                            send_std_msg(START_GAME, 0, 0, par[i].p2, NOT_TURN);
                            break;
                        }
                                   
                    }                 
                }
                else if(std_msg.cmd == BAD_ATTACK || std_msg.cmd == GOOD_ATACK){
                    printf("Message type %d sent by %d\n", std_msg.cmd, sd);
                    send_std_msg(std_msg.cmd, x, y, socket_to_sent, NOT_TURN);
                    printf("Message type %d sent to %d\n", std_msg.cmd, socket_to_sent);
                }
                else if(std_msg.cmd == ATACKING){
                    x = std_msg.x;
                    y = std_msg.y;

                    printf("Position attacked x: %d, y: %d\n", x, y);

                    if (sd == par[i/2].p2){
                        if(game_pared[i/2].my_player.my_map.positions[x][y] == 3 || game_pared[i/2].my_player.my_map.positions[x][y] == 4 || game_pared[i/2].my_player.my_map.positions[x][y] == 5){
                            
                            if(game_pared[i/2].my_player.my_map.positions[x][y] == 3) game_pared[i/2].my_player.my_ships[0].lifes--;
                            if(game_pared[i/2].my_player.my_map.positions[x][y] == 4) game_pared[i/2].my_player.my_ships[1].lifes--;
                            if(game_pared[i/2].my_player.my_map.positions[x][y] == 5) game_pared[i/2].my_player.my_ships[2].lifes--;

                            game_pared[i/2].my_player.my_map.positions[x][y] = 2;
                        }
                    }
                    else{
                        if(game_pared[i/2].rival_player.my_map.positions[x][y] == 3 || game_pared[i/2].rival_player.my_map.positions[x][y] == 4 || game_pared[i/2].rival_player.my_map.positions[x][y] == 5){
                            
                            if(game_pared[i/2].rival_player.my_map.positions[x][y] == 3) game_pared[i/2].rival_player.my_ships[0].lifes--;
                            if(game_pared[i/2].rival_player.my_map.positions[x][y] == 4) game_pared[i/2].rival_player.my_ships[1].lifes--;
                            if(game_pared[i/2].rival_player.my_map.positions[x][y] == 5) game_pared[i/2].rival_player.my_ships[2].lifes--;

                            game_pared[i/2].rival_player.my_map.positions[x][y] = 2;
                        }
                    }
                    printf("Message type %d sent by %d\n", std_msg.cmd, sd);
                    send_std_msg(std_msg.cmd, x, y, socket_to_sent, NOT_TURN);
                    printf("Message type %d sent to %d\n", std_msg.cmd, socket_to_sent);
                }                  
            }  
        }  
    } 

    free_fifo();

    printf("Sever shutdown\n");    
    return 0;  
}
