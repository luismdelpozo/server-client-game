#include <stdio.h>

#include "player.h"
#include "../config.h"
#include "ship.h"
#include "map.h"

/*int main(){
    PLAYER p1 = create_player(1);
    p1 = set_ships(p1);

    for (int x = 0; x < MAX_MAP_ROWS; x++){
        for(int y = 0; y < MAX_MAP_COLS; y++){
            printf("%d", p1.my_map.positions[x][y]);
        }
    }

    return 0;
}*/

PLAYER create_player(int id){
    PLAYER p;
    p.id = id;

    //Creamos las barcas
    SHIP s1 = create_ship(4, 4, "Battleship");
    SHIP s2 = create_ship(3, 3, "Fragate");
    SHIP s3 = create_ship(2, 2, "Cruiser");

    SHIP s[MAX_SHIPS] = {s1, s2, s3};

    for (int i = 0; i < MAX_SHIPS; i++){
        p.my_ships[i] = s[i];
    }

    //Creamos los mapas
    MAP m = create_map(id);
    p.my_map = m;
    p.rival_map = m;

    return p;
}

int is_inside_two_points(int d1, int d2, int dt){
    int t = d1 + d2;
    if(t == dt) return 1; // Si es igual es que esta dentro devolvemos 1        
    return 0;
}

int distance_two_points(int x1, int y1, int x2, int y2){
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

int pos_is_valid(int x, int y, int size, int ship_created[MAX_SHIPS][4]){
    //printf("Trying to set ship with size: %d x: %d, y: %d\n", size, x, y);
    int h1 = 0; // Horizontal hacia derecha
    int h2 = 0; // Horizontal hacia izquierda
    int v1 = 0; // Vertical hacia arriba
    int v2 = 0; // Vertical hacia abajo

    // -1: no se puede poner
    // 0: se puede poner horizontal derecha
    // 1: se puede poner horizontal izquierda
    // 2: se puede poner vertical abajo
    // 3: se puede poner vertical arriba

    // Primero comprobamos que no esta en el limite del mapa
    if((y + size) > (MAX_MAP_COLS)){
        h1 = 1;
    }
    if((y - size + 1) < 0){
        h2 = 1;
    }
    if((x + size) > (MAX_MAP_ROWS)){
        v1 = 1;
    }
    if((x - size + 1) < 0){
        v2 = 1;
    }
    //if(h1 == 1 || h2 == 1 || v1 == 1 || v2 == 1) 

    for(int q = 0; q < MAX_SHIPS; q++){
        //printf("Pos ship %d x0: %d y0: %d x1: %d y2: %d\n", q, ship_created[q][0], ship_created[q][1], ship_created[q][2], ship_created[q][3]);
        if ((x == ship_created[q][0] && y == ship_created[q][1]) || (x == ship_created[q][2] && y == ship_created[q][3]))
            return -1;
    }

    //printf("INIT sz: %d, h1: %d, h2: %d, v1: %d, v2: %d\n", size, h1, h2, v1, v2);

    for(int i = 0; i < MAX_SHIPS; i++){
        //printf("Is boat? %d\n", ship_created[i][0]);
        if (ship_created[i][0] != -1){
            for(int j = 0; j < size; j++){               
                if (h1 == 0){ // Horizontal derecha
                    int d1 = distance_two_points(ship_created[i][0], ship_created[i][1], x, y + j);
                    int d2 = distance_two_points(x, y + j, ship_created[i][2], ship_created[i][3]);
                    int dt = distance_two_points(ship_created[i][0], ship_created[i][1], ship_created[i][2], ship_created[i][3]);

                    h1 = is_inside_two_points(d1, d2, dt);
                    //printf("ERROR hori 1 %d\n", h1);
                }
                if(h2 == 0){ // Horizontal izquierda
                    int d1 = distance_two_points(ship_created[i][0], ship_created[i][1], x, y - j);
                    int d2 = distance_two_points(x, y - j, ship_created[i][2], ship_created[i][3]);
                    int dt = distance_two_points(ship_created[i][0], ship_created[i][1], ship_created[i][2], ship_created[i][3]);

                    h2 = is_inside_two_points(d1, d2, dt);
                    //printf("ERROR hori 2 %d\n", h2);
                }
                if(v1 == 0){ // Vertical abajo
                    int d1 = distance_two_points(ship_created[i][0], ship_created[i][1], x + j, y);
                    int d2 = distance_two_points(x + j, y, ship_created[i][2], ship_created[i][3]);
                    int dt = distance_two_points(ship_created[i][0], ship_created[i][1], ship_created[i][2], ship_created[i][3]);

                    v1 = is_inside_two_points(d1, d2, dt);
                    //printf("ERROR ver 1 %d\n", v1);
                }
                if (v2 == 0){ // Vertical arriba
                    int d1 = distance_two_points(ship_created[i][0], ship_created[i][1], x - j, y);
                    int d2 = distance_two_points(x - j, y, ship_created[i][2], ship_created[i][3]);
                    int dt = distance_two_points(ship_created[i][0], ship_created[i][1], ship_created[i][2], ship_created[i][3]);

                    v2 = is_inside_two_points(d1, d2, dt);
                    //printf("ERROR ver 2 %d\n", v2);
                }                
            } 
        }      
    }
    //printf("FINISH sz: %d, h1: %d, h2: %d, v1: %d, v2: %d\n", size, h1, h2, v1, v2);

    if(h1 == 0 && h2 == 0 && v1 == 0 && v2 == 0) return rand() % 4;
    else if(h1 == 1 && h2 == 1 && v1 == 1 && v2 == 1) return -1;
    else{
        while(1){
            int num = rand() % 4;
            if(num == 0 && h1 == 0) return 0;
            else if(num == 1 && h2 == 0) return 1;
            else if(num == 2 && v1 == 0) return 2;
            else if(num == 3 && v2 == 0) return 3;
        }
    }
}

PLAYER set_ships(PLAYER p1){
    int map_character[3] = {3, 4, 5};

    int ship_created[MAX_SHIPS][4];
    for (int i = 0; i < MAX_SHIPS; i++){
        ship_created[i][0] = -1;
        ship_created[i][1] = -1;
        ship_created[i][2] = -1;
        ship_created[i][3] = -1;
    }

    time_t t;
    srand((unsigned) time(&t));

    for (int s = 0; s < MAX_SHIPS; s++){

        int num_row = rand() % MAX_MAP_ROWS;
        int num_col = rand() % MAX_MAP_COLS;
        int size = p1.my_ships[s].size;
        int r = pos_is_valid(num_row, num_col, size, ship_created);

        while(r == -1){    
            //printf("r: %d\n", r);        
            num_row = rand() % MAX_MAP_ROWS;
            num_col = rand() % MAX_MAP_COLS;
            size = p1.my_ships[s].size;
            r = pos_is_valid(num_row, num_col, size, ship_created);
        }
        
        if (r == 0){ //Horizontal derecha                             
            p1.my_ships[s].pos[1].y = num_col + size;
            p1.my_ships[s].pos[0].x = num_row;
            p1.my_ships[s].pos[0].y = num_col;
            p1.my_ships[s].pos[1].x = num_row;

            ship_created[s][0] = num_row;
            ship_created[s][1] = num_col;
            ship_created[s][2] = num_row;
            ship_created[s][3] = num_col + size;
        }
        else if(r == 1){ // Horizontal izquierda
            p1.my_ships[s].pos[1].y = num_col - size;
            p1.my_ships[s].pos[0].x = num_row;
            p1.my_ships[s].pos[0].y = num_col;
            p1.my_ships[s].pos[1].x = num_row;

            ship_created[s][0] = num_row;
            ship_created[s][1] = num_col;
            ship_created[s][2] = num_row;
            ship_created[s][3] = num_col - size;
        }       
        else if (r == 2){ // Vertical abajo                             
            p1.my_ships[s].pos[1].x = num_row + size;
            p1.my_ships[s].pos[0].x = num_row;
            p1.my_ships[s].pos[0].y = num_col;
            p1.my_ships[s].pos[1].y = num_col;

            ship_created[s][0] = num_row;
            ship_created[s][1] = num_col;
            ship_created[s][2] = num_row + size;
            ship_created[s][3] = num_col;
        }
        else if(r == 3){ // Vertical arriba
            p1.my_ships[s].pos[1].x = num_row - size;
            p1.my_ships[s].pos[0].x = num_row;
            p1.my_ships[s].pos[0].y = num_col;
            p1.my_ships[s].pos[1].y = num_col;

            ship_created[s][0] = num_row;
            ship_created[s][1] = num_col;
            ship_created[s][2] = num_row - size;
            ship_created[s][3] = num_col;

        }       
        
        for(int n = 0; n < size; n++){
            // Cambiamos el mapa
            if(r == 0){
                p1.my_map.positions[num_row][num_col + n] = map_character[s];
            }
            else if(r == 1){
                p1.my_map.positions[num_row][num_col - n] = map_character[s];
            }
            else if(r == 2){
                p1.my_map.positions[num_row + n][num_col] = map_character[s];
            }
            else if(r == 3){
                p1.my_map.positions[num_row - n][num_col] = map_character[s];
            }
        }
    }
    return p1;
}

int player_deleted(PLAYER p1){
    for(int i = 0; i < MAX_SHIPS; i++){
        if(p1.my_ships[i].lifes > 0){
            return 1;
        }
    }
    return 0;
}
