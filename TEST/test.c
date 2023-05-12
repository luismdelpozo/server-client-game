#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FINISH "000"

struct msg_new_player{
    int cmd;
    int id;
    //int map_positions[MAX_MAP_ROWS][MAX_MAP_COLS];
};

int main(int argc, char** argv)
{
   struct msg_new_player my_data;
   my_data.cmd = 1;
   my_data.id = 4;

   char buffer[20];
   memcpy(buffer, &my_data, sizeof(my_data));
   printf("%s\n", buffer);

   struct msg_new_player copy_data;
   memcpy(&copy_data, buffer, sizeof(copy_data));

   printf("the numbers : %d - %d\n", copy_data.cmd, copy_data.id);



   return 0;
}