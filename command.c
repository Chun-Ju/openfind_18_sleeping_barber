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

   // opening the IPC semaphore
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
   printf("------------------------------------------\n");
   printf("PLEASE ENTER a OR c THEN ENTER ONLY\n");
   printf("command 'a': means customers arrive\n");
   printf("command 'c': means time to close the store\n");
   printf("------------------------------------------\n");

   /* ------------------------------------------------   *
    * receiving the comamnd inputed by user by Felicia   *
    * ------------------------------------------------   */
   while (open) {
      printf("Command:");

      char  *line = NULL;
      size_t len = 0;
      ssize_t readSize;
      readSize = getline(&line, &len, stdin);

      if (readSize == -1){

         perror("Error(getline):");
         result = ERR_GETLINE;
         goto error_handling_c1;

      } else if ((readSize - 1) != 1) { //command length not correct

         if (readSize == 1) { // only '\n'
            printf("COMMAND CANNOT BE EMPTY!!\n");
         } else { // must too long
            printf("COMMAND IS TOOOO LONG!!\n");
         }

      } else if (line[0] == 'a') { //customers arrive
         sem_post(SEM_arrive);
      } else if (line[0] == 'c'){ //signal to close the store and notify barbers to sleep

         for(int  i = 0; i < 1 + BARBER_COUNT; i++){//1 for close store, others for barber
            sem_post(SEM_closeAll);
         }
         sem_post(SEM_arrive);
         break;

      } else { // error command
         printf("ERROR COMMAND, PLEASE ENTER a OR c THEN ENTER\n");
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
