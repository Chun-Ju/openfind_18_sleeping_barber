#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

#include "defineConstant.h"

sem_t *SEM_closeAll;
sem_t *SEM_chair;
sem_t *SEM_sleepingBarber;
sem_t *SEM_customers;
sem_t *SEM_order;

int
main(){
   /* -------------------------  *
    * Initialization by Felicia  *
    * -------------------------  */
   int   result = SUCCESS;

   // opening the IPC semaphore
   sem_t *SEM_arrive = sem_open("SEM_arrive", O_CREAT, 0664, 0);
   if (!SEM_arrive) {
      perror("Error(sem_open):");
      return ERR_SEM_OPEN;
   }

   SEM_closeAll = sem_open("SEM_closeAll", O_CREAT, 0664, 0);
   if (!SEM_closeAll) {
      perror("Error(sem_open):");
      result = ERR_SEM_OPEN;
      goto error_handling_b0;
   }

   // global semaphore
   SEM_chair = sem_open("SEM_chair", O_CREAT, 0664, 1);
   if (!SEM_chair) {
      perror("Error(sem_open):");
      result = ERR_SEM_OPEN;
      goto error_handling_b1;
   }

   SEM_sleepingBarber = sem_open("SEM_sleepingBarber", O_CREAT, 0664, 1);
   if (!SEM_sleepingBarber) {
      perror("Error(sem_open):");
      result = ERR_SEM_OPEN;
      goto error_handling_b2;
   }

   SEM_customers = sem_open("SEM_customers", O_CREAT, 0664, 0);
   if (!SEM_customers) {
      perror("Error(sem_open):");
      result = ERR_SEM_OPEN;
      goto error_handling_b3;
   }

   SEM_order = sem_open("SEM_order", O_CREAT, 0664, 1);
   if (!SEM_order) {
      perror("Error(sem_open):");
      result = ERR_SEM_OPEN;
      goto error_handling_b4;
   }

   /* ------------------------------------------------------   *
    * closing and unlinking the opening semaphore by Felicia   *
    * ------------------------------------------------------   */
   sem_close(SEM_order);
   sem_unlink("SEM_order");
error_handling_b4:
   sem_close(SEM_customers);
   sem_unlink("SEM_customers");
error_handling_b3:
   sem_close(SEM_sleepingBarber);
   sem_unlink("SEM_sleepingBarber");
error_handling_b2:
   sem_close(SEM_chair);
   sem_unlink("SEM_chair");
error_handling_b1:
   sem_close(SEM_closeAll);
   sem_unlink("SEM_closeAll");
error_handling_b0:
   sem_close(SEM_arrive);
   sem_unlink("SEM_arrive");

   return result;
}
