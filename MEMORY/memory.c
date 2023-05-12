#ifndef MEMORY_C
#define MEMORY_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memory.h"

int get_max_game_id(){
    // get the last game id saved in the memory
    if (access(MEM_PATH_GAME, F_OK) < 0){
        printf("File %s does not exist\n", MEM_PATH_GAME);
        return 1;
    }

    GAME g;
    FILE *fptr;
    fptr = fopen(MEM_PATH_GAME,"r");

    int max_id = 0;

    fseek(fptr, 0, SEEK_END);
    int elems = ftell(fptr)/sizeof(GAME);
    fseek(fptr, 0, SEEK_SET);
    
    for (int i = 0; i < elems; i++){ 
        fseek(fptr,sizeof(GAME)*i,SEEK_SET);
        fread(&g,sizeof(GAME),1,fptr);
        
        if(g.id > max_id) max_id = g.id;
    }
    fclose(fptr);

    return max_id;
}

void read_all_games(void){
    // Read all the ids from all the games saved
    if (access(MEM_PATH_GAME, F_OK) < 0){
        printf("File %s does not exist\n", MEM_PATH_GAME);
        exit(EXIT_FAILURE);
    }

    GAME g;
    FILE *fptr;
    fptr = fopen(MEM_PATH_GAME,"r");

    fseek(fptr, 0, SEEK_END);
    int elems = ftell(fptr)/sizeof(GAME);
    fseek(fptr, 0, SEEK_SET);

    printf("Numero de partidas guardadas: %d\n", elems);
    printf("Que partida quieres cargar?\n");
    
    for (int i = 0; i < elems; i++){ 
        fseek(fptr,sizeof(GAME)*i,SEEK_SET);
        fread(&g,sizeof(GAME),1,fptr);
        printf("%d - GAME con id %d\n", i, g.id);
    }
    fclose(fptr);
}

int write_game(GAME g){
    // Save game in binary file
    FILE *fptr;

    if(access(MEM_PATH_GAME, F_OK) < 0){
        printf("File %s does not exist\n", MEM_PATH_GAME);
        printf("Creating the file %s...\n", MEM_PATH_GAME);

        fptr = fopen(MEM_PATH_GAME, "w+");
    }
    else{
        fptr = fopen(MEM_PATH_GAME, "a");
    }    
    
    if(fptr == NULL){
        printf("Error openning the file %s\n", MEM_PATH_GAME);   
        exit(EXIT_FAILURE);         
    }

    fwrite(&g, sizeof(GAME), 1, fptr);
    fclose(fptr);
    printf("Partida %d guardada correctamente\n", g.id);
    return 0;
}

GAME read_game(int id){
    // Read game with one id saved in the memory (binary files)
    if (access(MEM_PATH_GAME, F_OK) < 0){
        printf("File %s does not exist\n", MEM_PATH_GAME);
        exit(EXIT_FAILURE);
    }

    FILE *fptr;
    GAME last_g;

    fptr = fopen(MEM_PATH_GAME, "r");

    if(fptr == NULL){
        printf("Error openning the file %s\n", MEM_PATH_GAME);   
        exit(EXIT_FAILURE);             
    }

    fseek(fptr, sizeof(GAME) * (id - 1), SEEK_SET);
    fread(&last_g, sizeof(GAME), 1, fptr);
    fclose(fptr);

    printf("Partida %d leida correctamente de la memoria\n", last_g.id);
    return last_g;
}

#endif