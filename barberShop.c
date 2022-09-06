#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/errno.h>

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

int   freeChairCounts = CHAIR_COUNT;
int   head = 0;
CHAIR waitingQueue[CHAIR_COUNT];

void  *
THD_barber(void *Bid){
   /* ------------------------------------   *
    * barber opens its own door by Felicia   *
    * ------------------------------------   */
   int   bid = *(int*) Bid;
   printf("barber%d opens the door\n", bid);
   int   ret = sem_post(SEM_order);
   if (ret == -1) {
      pthread_exit(NULL);
   }

   /* ----------------------------------------------------------------------------------- *
    * barber waits arrivers come to serve the sustomers or close its own store by Felicia *
    * ----------------------------------------------------------------------------------- */
   while(1){
      ret = sem_wait(SEM_customers);
      if (ret == -1) {
         pthread_exit(NULL);
      }

      ret = sem_trywait(SEM_closeAll);
      if (ret == 0) { // close store
         break;
      } else if (ret == -1 && errno != EAGAIN) {
         pthread_exit(NULL);
      }

      ret = sem_wait(SEM_chair);
      if (ret == -1) { // fail to get customers info so didn't serve
         ret = sem_post(SEM_customers);
         if (ret == -1) { // it may lose one customers
            pthread_exit(NULL);
         }
         pthread_exit(NULL);
      }

      // deQ to get the customer's Info
      if (freeChairCounts == CHAIR_COUNT){
         printf("Error: queue is empty\n");
      }
      int   cid = waitingQueue[head].cid;
      head = (head + 1) % CHAIR_COUNT;
      // release one chair in waiting room
      freeChairCounts += 1;
      printf("barber%d is serving customer%d\n", bid, cid);
      ret = sem_post(SEM_chair);
      if (ret == -1) {
         pthread_exit(NULL);
      }
      // barber is cutting the customer's hair
      sleep(4);
      printf("barber%d has finished serving customer%d, so customer%d leaves.\n", bid, cid, cid);
      // telling the customers who is waiting on the bench that barber's idling now.
      ret = sem_post(SEM_sleepingBarber);
      if (ret == -1) {
         pthread_exit(NULL);
      }
   }
   printf("barber%d sleeps\n", bid);
   pthread_exit(NULL);
}

void *
THD_customer(void  *Cid){
   int   cid = *(int*) Cid;

   int   ret = sem_wait(SEM_chair);
   if (ret == -1) {
      pthread_exit(NULL);
   }

   if (freeChairCounts <= 0) {// no seats
      printf("customer%d leaves due to no seats in waiting room\n", cid);
      ret = sem_post(SEM_chair);
      if (ret == -1) {
         pthread_exit(NULL);
      }

      ret = sem_post(SEM_order);
      if (ret == -1) {
         pthread_exit(NULL);
      }
   } else {
      // TODO insert into waitinig queue
      int insertIndex = (head + (CHAIR_COUNT - freeChairCounts)) % CHAIR_COUNT;
      waitingQueue[insertIndex].cid = cid;
      waitingQueue[insertIndex].tid = pthread_self();

      freeChairCounts -= 1;
      printf("customer%d waiting in the bench\n", cid);

      ret = sem_post(SEM_customers);
      if (ret == -1) {
         pthread_exit(NULL);
      }

      ret = sem_post(SEM_chair);
      if (ret == -1) {
         pthread_exit(NULL);
      }

      ret = sem_post(SEM_order);
      if (ret == -1) {
         pthread_exit(NULL);
      }

      ret = sem_wait(SEM_sleepingBarber);
      if (ret == -1) {
         pthread_exit(NULL);
      }
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

   SEM_sleepingBarber = sem_open("SEM_sleepingBarber", O_CREAT, 0664, 0);
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
      int   ret = sem_wait(SEM_order);
      if (ret == -1){
         perror("Error(sem_wait)\n");
         result = ERR_SEM_WAIT;
         goto error_handling_b5;
      }
      ++barberId;
      ret = pthread_create(&barberThread[i], NULL, THD_barber, &barberId);
      if (ret != 0){
         perror("Error(pthread_create)\n");
         result = ERR_PTHD_CREATE;
         goto error_handling_b5;
      }
   }

   /* ---------------------------------------   *
    * create the customer (Thread) by Felicia   *
    * ---------------------------------------   */
   int         customerId = 0;
   while(1){
      int   ret = sem_wait(SEM_arrive);
      if (ret == -1) {
         perror("Error(sem_wait)\n");
         result = ERR_SEM_WAIT;
         goto error_handling_b5;
      }
      ret = sem_trywait(SEM_closeAll);
      if (ret == -1 && errno != EAGAIN) {
         perror("Error(sem_trywait)\n");
         result = ERR_SEM_WAIT;
         goto error_handling_b5;
      } else if(ret == 0){
         ret = sem_wait(SEM_chair);
         if (ret == -1){
            perror("Error(sem_wait)\n");
            result = ERR_SEM_WAIT;
            goto error_handling_b5;
         }

         printf("------------------------------\n");
         printf("|  %s  |\n","ready to close the store");
         printf("------------------------------\n");

         char  benchStr[BENCH_STR_LEN] = "";
         // pthread_cancel the customers wait in the waiting room
         for(int  i = 0; i < CHAIR_COUNT - freeChairCounts; i++){
            if (freeChairCounts == CHAIR_COUNT){
               printf("Error: queue is empty\n");
            }
            pthread_t   tid = waitingQueue[head].tid;
            int         cid = waitingQueue[head].cid;
            char        cidStr[INT_MAX_DIG + 2]; // 1 for ',', 1 for '\0'
            int         cidStrLen = snprintf(cidStr, INT_MAX_DIG + 1, "%d", cid);
            cidStr[cidStrLen++] = ',';
            cidStr[cidStrLen++] = '\0';
            strncat(benchStr, cidStr, cidStrLen);
            head = (head + 1) % CHAIR_COUNT;
            pthread_cancel(tid);
         }
         if(strlen(benchStr) != 0){
            benchStr[strlen(benchStr) - 1] = '\0';//remove redundant ','
            printf("dismiss customer %s in the bench.\n", benchStr);
         }else{
            printf("NO CUSTOMER IS WAITING IN THE BENCH.\n");
         }
         ret = sem_post(SEM_chair);
         if (ret == -1){
            perror("Error(sem_post)\n");
            result = ERR_SEM_POST;
            goto error_handling_b5;
         }
         break;
      }
      ret = sem_wait(SEM_order);
      if (ret == -1){
         perror("Error(sem_wait)\n");
         result = ERR_SEM_WAIT;
         goto error_handling_b5;
      }
      printf("customer%d arrives the store\n", ++customerId);
      pthread_t customerThread;
      ret = pthread_create(&customerThread, NULL, THD_customer, &customerId);
      if (ret != 0){
         perror("Error(pthread_create)\n");
         result = ERR_PTHD_CREATE;
         goto error_handling_b5;
      }
   }

   /* ----------------------------------------------------------------- *
    * dispatch the people to tell barber it is time to sleep by Felicia *
    * ----------------------------------------------------------------- */
   for(int i = 0; i < BARBER_COUNT; i++){
      int   ret = sem_post(SEM_customers);
      if (ret == -1){
         perror("Error(sem_post)\n");
         result = ERR_SEM_POST;
         goto error_handling_b5;
      }
   }
   for(int i = 0; i < BARBER_COUNT; i++){
      pthread_join(barberThread[i], NULL);
   }
   printf("close\n");
   /* ------------------------------------------------------   *
    * closing and unlinking the opening semaphore by Felicia   *
    * ------------------------------------------------------   */
error_handling_b5:
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
