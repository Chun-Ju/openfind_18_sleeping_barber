#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

#include "defineConstant.h"

int
main(){
   /* -------------------------  *
    * Initialization by Felicia  *
    * -------------------------  */
   int   result = SUCCESS;

   //opening the IPC semaphore by Felicia
   sem_t *SEM_arrive = sem_open("SEM_arrive", O_CREAT, 0664, 0);
   if (!SEM_arrive) {
      perror("Error(sem_open):");
      return ERR_SEM_OPEN;
   }

   sem_t *SEM_closeAll = sem_open("SEM_closeAll", O_CREAT, 0664, 0);
   if (!SEM_closeAll) {
      perror("Error(sem_open):");
      result = ERR_SEM_OPEN;
      goto error_handling_b0;
   }

   /* ------------------------------------------------------   *
    * closing and unlinking the opening semaphore by Felicia   *
    * ------------------------------------------------------   */
   sem_close(SEM_closeAll);
   sem_unlink("SEM_closeAll");
error_handling_b0:
   sem_close(SEM_arrive);
   sem_unlink("SEM_arrive");

   return result;
}
