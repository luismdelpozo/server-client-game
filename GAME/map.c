#include "map.h"

#define MAX_BUF_MAP 1024

MAP create_map(int id){
    MAP m;
    m.id = id;
    for(int i = 0; i < MAX_MAP_ROWS; i++){
        for(int j = 0; j < MAX_MAP_COLS; j++){
            m.positions[i][j] = 0;
        }
    }
    return m;
}

MAP change_map(MAP m, int x, int y, int value){
    m.positions[x][y] = value;
    return m;
}

void print_map(MAP m){
    char printed_map[MAX_BUF_MAP] = "";

    strcat(printed_map, "     A   B   C   D   E   F   G   H   I   J\n");
    strcat(printed_map, "    _______________________________________\n");

    for(int i = 0; i < MAX_MAP_ROWS; i++){
        char c[10];        
        if (i < 10) sprintf(c, "  %d", i);
        else sprintf(c, " %d", i);

        strcat(printed_map, c);
        strcat(printed_map, "|");

        for(int j = 0; j < MAX_MAP_COLS; j++){
            if(m.positions[i][j] == 0){
                strcat(printed_map, "___|");
            }
            else if(m.positions[i][j] == 1){
                strcat(printed_map, "SSS|");
            }
            else if(m.positions[i][j] == 2){
                strcat(printed_map, "VVV|");
            }
            else if(m.positions[i][j] == 3){
                strcat(printed_map, "BBB|");
            }
            else if(m.positions[i][j] == 4){
                strcat(printed_map, "FFF|");
            }
            else if(m.positions[i][j] == 5){
                strcat(printed_map, "CCC|");
            }
        }
        strcat(printed_map, "\n");
    }
    
    strcat(printed_map, "\n   |SSS| --> attacked failed\n");
    strcat(printed_map, "   |VVV| --> right attacked\n");
    strcat(printed_map, "   |BBB| --> Battleship positioned\n");
    strcat(printed_map, "   |FFF| --> Fragate positioned\n");
    strcat(printed_map, "   |CCC| --> Cruiser positioned\n");
    printf("%s", printed_map);
}