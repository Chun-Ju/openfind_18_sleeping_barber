#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>

#include "defineConstant.h"

sem_t *SEM_closeAll;
sem_t *SEM_chair;
sem_t *SEM_sleepingBarber;
sem_t *SEM_customers;
sem_t *SEM_order;

typedef struct chair{
   int cid;
   pthread_t tid;
}CHAIR;

int freeChairCounts= CHAIR_COUNT;
int head = 0;
CHAIR waitingQueue[CHAIR_COUNT];

void  *
barber(void *Bid){
   /* ------------------------------------   *
    * barber opens its own door by Felicia   *
    * ------------------------------------   */
   int   bid = *(int*) Bid;
   printf("barber%d opens the door\n", bid);
   sem_post(SEM_order);

   /* ----------------------------------------------------------------------------------- *
    * barber waits arrivers come to serve the sustomers or close its own store by Felicia *
    * ----------------------------------------------------------------------------------- */
   while(1){
      sem_wait(SEM_customers);
      int   ret = sem_trywait(SEM_closeAll);
      if(ret == 0){
         break;
      }
      sem_wait(SEM_chair);
      // deQ to get the customer's Info
      if (freeChairCounts == CHAIR_COUNT){
         printf("Error: queue is empty\n");
      }
      int   cid = waitingQueue[head].cid;
      head = (head + 1) % CHAIR_COUNT;
      // release one chair in waiting room
      freeChairCounts += 1;
      printf("barber%d is serving customer%d\n", bid, cid);
      sem_post(SEM_chair);
      // barber is cutting the customer's hair
      sleep(4);
      printf("customer%d leaves due to barber%d has finished serving customer%d.\n", cid, bid, cid);
      // telling the customers who is waiting on the bench that barber's idling now.
      sem_post(SEM_sleepingBarber);
   }
   printf("barber%d sleeps\n", bid);
   pthread_exit(NULL);
}

void *
customer(void  *Cid){
   int cid = *(int*) Cid;
   sem_wait(SEM_chair);
   if (freeChairCounts <= 0) {// no seats
      printf("customer%d leaves due to no seats in waiting room\n", cid);
      sem_post(SEM_chair);
      sem_post(SEM_order);
   } else {
      // TODO insert into waitinig queue
      int insertIndex = (head + (CHAIR_COUNT - freeChairCounts)) % CHAIR_COUNT;
      waitingQueue[insertIndex].cid = cid;
      waitingQueue[insertIndex].tid = pthread_self();

      freeChairCounts -= 1;
      printf("customer%d waiting in the bench\n", cid);
      sem_post(SEM_customers);
      sem_post(SEM_chair);
      sem_post(SEM_order);
      sem_wait(SEM_sleepingBarber);
   }

   pthread_exit(NULL);
}

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

   /* -------------------------------------  *
    * create the barber (Thread) by Felicia  *
    * -------------------------------------  */
   int         barberId = 0;
   pthread_t   barberThread[BARBER_COUNT];
   // <BARBER_COUNT> barber start working
   for(int i = 0; i < BARBER_COUNT; i++){
      sem_wait(SEM_order);
      ++barberId;
      pthread_create(&barberThread[i], NULL, barber, &barberId);
   }

   /* -------------------------------------  *
    * create the barber (Thread) by Felicia  *
    * -------------------------------------  */
   int         customerId = 0;
   while(1){
      sem_wait(SEM_arrive);
      int ret = sem_trywait(SEM_closeAll);
      if(ret == 0){
         // pthread_cancel the customers wait in the waiting room

         break;
      }
      sem_wait(SEM_order);
      ++customerId;
      printf("customer%d arrives the store\n", customerId);
      pthread_t customerThread;
      pthread_create(&customerThread, NULL, customer, &customerId);
   }

   /* ----------------------------------------------------------------- *
    * dispatch the people to tell barber it is time to sleep by Felicia *
    * ----------------------------------------------------------------- */
   for(int i = 0; i < BARBER_COUNT; i++){
      sem_post(SEM_customers);
   }
   for(int i = 0; i < BARBER_COUNT; i++){
      pthread_join(barberThread[i], NULL);
   }
   printf("close\n");
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
