#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include "defineConstant.h"

int
main(){
   /* -------------------------  *
    * Initialization by Felicia  *
    * -------------------------  */
   _Bool open = TRUE;
   int result = SUCCESS;

   // opening the IPC semaphore by Felicia
   sem_t *SEM_arrive = sem_open("SEM_arrive", O_CREAT, 0664, 0);
   if (!SEM_arrive) {
      perror("Error(sem_open):");
      return ERR_SEM_OPEN;
   }

   sem_t *SEM_closeAll = sem_open("SEM_closeAll", O_CREAT, 0664, 0);
   if (!SEM_closeAll) {
      perror("Error(sem_open):");
      result = ERR_SEM_OPEN;
      goto error_handling_c0;
   }
   /* ------------------------------------------------   *
    * receiving the comamnd inputed by user by Felicia   *
    * ------------------------------------------------   */
   while (open) {
      char  command[COMMAND_LEN];
      printf("Command:");
      scanf(" %s", command);
      if (strlen(command) != 1) { //command too long
         printf("Error(command format): it's too long\n");
         result = ERR_ARGS;
         goto error_handling_c1;
      } else if (command[0] == 'a') { //customers arrive
         sem_post(SEM_arrive);
      } else if (command[0] == 'c'){ //signal to close the store and notify barbers to sleep
         for(int  i = 0; i < 1 + BARBER_COUNT; i++){//1 for close store, others for barber
            sem_post(SEM_closeAll);
         }
         sem_post(SEM_arrive);
         break;
      } else {
         printf("Error(command format): error cpmmand, please enter a or c and enter only\n");
         result = ERR_ARGS;
         goto error_handling_c1;
      }
   }

   /* ------------------------------------------------------   *
    * closing and unlinking the opening semaphore by Felicia   *
    * ------------------------------------------------------   */
error_handling_c1:
   sem_close(SEM_closeAll);
   sem_unlink("SEM_closeAll");
error_handling_c0:
   sem_close(SEM_arrive);
   sem_unlink("SEM_arrive");

   return result;
}
