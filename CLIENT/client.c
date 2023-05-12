#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
 
#include "../config.h"
#include "../GAME/player.h"
#include "../GAME/game.h" 
#include "../SERVER/aux_func.h"

/*
    COMPILATION CLIENT.C FILE:
    
    gcc -Wall 
    ../config.h 
    ../GAME/ship.h       ../GAME/ship.c 
    ../GAME/map.h        ../GAME/map.c 
    ../GAME/player.h     ../GAME/player.c 
    ../GAME/game.h       ../GAME/game.c 
    ../MEMORY/memory.h   ../MEMORY/memory.c 
    ../SERVER/aux_func.h ../SERVER/aux_func.c
    client.c -o client -lm
*/

int main()
{
    GAME game;
    PLAYER p1 = create_player(0);
    PLAYER p2 = create_player(0);
    game = create_game(0, p1, p2);
    
    struct sockaddr_in serverAddr;
    int clientSocket, ret, n_bytes_recv, turn; 

    int size_std_msg = sizeof(struct msg_std);
    int size_msg_new_pl = sizeof(struct msg_new_player);

    //char turn[MAX_BUFF_MEMORY];
    char *ptr;
    int x,y, elems, correct_turno;
    
    clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (clientSocket < 0) {
        printf("Error in connection 0.\n");
        exit(EXIT_FAILURE);
    }    
 
    // Assigning port number and IP address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
 
    ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); 
    if (ret < 0) {
        printf("Error in connection 1.\n");
        exit(EXIT_FAILURE);
    }

    char msg_start[size_msg_new_pl];    
    n_bytes_recv = recv(clientSocket, msg_start, size_msg_new_pl, 0);
    if (n_bytes_recv < 0) {
        printf("Error in receiving data.\n");
        exit(EXIT_FAILURE);
    }
    
    struct msg_new_player new_pl_msg;
    memcpy(&new_pl_msg, msg_start, sizeof(new_pl_msg)); 

    game.my_player.id = new_pl_msg.id;
    for(int x = 0; x < MAX_MAP_ROWS; x++){
        for(int y = 0; y < MAX_MAP_COLS; y++){
            game.my_player.my_map.positions[x][y] = new_pl_msg.map_positions[x][y];
        }
    }

    if(new_pl_msg.cmd == WAIT_SERVER){
        printf("Esperando a que se libere el servidor de otros clientes...\n");
    } 
    else if(new_pl_msg.cmd == WAIT_PLAYER){
        printf("Conectando con otro jugador...\n");
    }

    char msg_start_2[size_msg_new_pl]; 
    n_bytes_recv = recv(clientSocket, msg_start_2, size_msg_new_pl, 0);
    if (n_bytes_recv < 0) {
        printf("Error in receiving data wait player\n");
        exit(EXIT_FAILURE);
    }
    
    struct msg_std my_msg;
    memcpy(&my_msg, msg_start_2, n_bytes_recv);
    if (new_pl_msg.cmd == START_GAME){
        printf("Starting game %d...\n", game.my_player.id);
        //break;
    }    

    /*if (game.my_player.id % 2 == 0) turn = 0;
    else turn = 1;*/
    turn = my_msg.turn;

    int finish = 0; 

    while(finish == 0){
        if (turn == YES_TURN){           
            correct_turno = 0;
            // Printeamos mapas de los jugadores para ver donde tirar
            print_map(game.my_player.my_map);
            print_map(game.rival_player.my_map);

            while(correct_turno == 0){
                printf("Es tu turno, posibles comandos a enviar:\n");
                printf("a - FF para finalizar partida\n");
                printf("b - XY coordenadas en el rango X -> [0, 11] && Y -> [A, L]\n");
                printf("msg > ");

                char str[3];            
                scanf("%s", str);
                //printf("%s\n", str);
                ptr = &str[0];
                
                if(strncmp(str, "FF", 2) == 0) {
                    printf("Finalizando la partida...\n");
                    correct_turno = 1;
                    finish = 1;

                    send_std_msg(FINISH, 0, 0, clientSocket, YES_TURN);
                    printf("JUEGO FINALIZADO\n");                    
                }
                else{
                    elems = sizeof(str)/sizeof(char);
                    if(elems == 2 || elems == 3){
                        x = (int) *ptr - 48; // char "0" se traduce a 48
                        ptr++;
                        y = (int) *ptr - 65; // char "A" se traduce a 65

                        if ((x >= 0 && x <= MAX_MAP_ROWS) && (y >= 0 && y <= MAX_MAP_COLS)){
                            // Coordenadas correctas, enviar por sockets
                            printf("Enviando posicion x: %d, y: %d\n", x, y);
                            send_std_msg(ATACKING, x, y, clientSocket, YES_TURN);
                            printf("Esperando resupuesta del otro jugador...\n");

                            struct msg_std std_msg;
                            char msg[sizeof(std_msg)];
                            n_bytes_recv = read(clientSocket, msg, sizeof(std_msg));
                            if (n_bytes_recv < 0){  
                                printf("Error receiving data from server\n");
                                exit(EXIT_FAILURE);
                            }
                            
                            memcpy(&std_msg, msg, sizeof(std_msg));

                            if(std_msg.cmd == GOOD_ATACK){
                                printf("Has acertado el ataque!\n");
                                game.my_player.rival_map.positions[x][y] = 2;
                            }                            
                            else if(std_msg.cmd == BAD_ATTACK){
                                printf("Ataque fallido!\n");
                                game.my_player.rival_map.positions[x][y] = 1;
                            }
                            else if(std_msg.cmd == LOSE){
                                printf("HAS GANAGO!\n");
                                game.my_player.rival_map.positions[x][y] = 2;
                                finish = 1;
                            }
                            
                            correct_turno = 1;
                            turn = NOT_TURN;
                        }
                        else{
                            printf("Has introducido mal las coordenadas del ataque\n");
                        }
                    }                
                }
            }
        }
        else{
            printf("Esperando el turno del rival...\n");

            char msg_nt[size_std_msg];

            n_bytes_recv =  read(clientSocket, msg_nt, size_std_msg);
            if(n_bytes_recv < 0){
                printf("Recv call not my turn function failed\n");
                exit(EXIT_FAILURE);
            }
            //printf("Mensaje recibido : %s\n", msg);
            struct msg_std std_msg;
            memcpy(&std_msg, msg_nt, n_bytes_recv);
            
            if (std_msg.cmd == FINISH){
                printf("el rival se ha desconectado de la partida\n");
                finish = 1;
            }
            else if(std_msg.cmd == ATACKING){
                x = std_msg.x;
                y = std_msg.y;
                printf("El rival ha atacado a la posicion: %d %d\n", x, y);     

                if(game.my_player.my_map.positions[x][y] == 3 || game.my_player.my_map.positions[x][y] == 4 || game.my_player.my_map.positions[x][y] == 5){
                    printf("El rival ha atacado tu barco!\n");
                    if(game.my_player.my_map.positions[x][y] == 3) game.my_player.my_ships[0].lifes--;
                    if(game.my_player.my_map.positions[x][y] == 4) game.my_player.my_ships[1].lifes--;
                    if(game.my_player.my_map.positions[x][y] == 5) game.my_player.my_ships[2].lifes--;

                    game.my_player.my_map.positions[x][y] = 2;

                    if(player_deleted(game.my_player) == 0){
                        // Jugador eliminado
                        printf("Estás eliminado, todos tus barcos han sido derrotados.\n");
                        send_std_msg(LOSE, 0, 0, clientSocket, NOT_TURN);
                        finish = 1;
                    }
                    else{
                        send_std_msg(GOOD_ATACK, 0, 0, clientSocket, NOT_TURN);
                    }
                }
                else{
                    printf("El rival ha fallado su ataque!\n");
                    game.my_player.my_map.positions[x][y] = 1;
                    send_std_msg(BAD_ATTACK, 0, 0, clientSocket, NOT_TURN);
                }
                turn = YES_TURN;
            }                       
        }
    }

    printf("Client shutdown\n");
    close(clientSocket);
    return 0;
}