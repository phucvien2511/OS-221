#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistdio.h>
#include <unistd.h>
#define MAX_ITEMS 1
#define LOOPS 1 * MAX_ITEMS
#define THREADS 3
sem_t S0,S1,S2;
int x = 0;
void *F1(void* arg) {
   int a = 0;
   while (a < 10) {
      sem_wait(&S0);
      for (int i = 0; i < 100000000; i++) x++;
      sem_post(&S1); 
      a++;   
   }
   pthread_exit(NULL);
}
void *F2(void* arg) {
   int a = 0;
   while (a < 10) {
      sem_wait(&S1);
      for (int i = 0; i < 100000000; i++) x++;
      sem_post(&S0);
      a++;
   }
}
void *F3(void* arg) {
   
   while (1) {
      sem_wait(&S2);
      printf("From F3\n");
      sleep(1);
      sem_post(&S0);
   }
   pthread_exit(NULL);
}
int main() {
   int tid[THREADS];
   pthread_t p1;
   pthread_t p2;
   pthread_t p3;
   sem_init(&S0, 0, 1);
   sem_init(&S1, 0, 0);
   // sem_init(&S2, 0, 0);
   pthread_create(&p1, NULL, F1, NULL);
   pthread_create(&p2, NULL, F2, NULL);
   // pthread_create(&p3, NULL, F3, NULL);

   pthread_join(p1, NULL);
   pthread_join(p2, NULL);
   printf("%d", x);
   // pthread_join(p3, NULL);
   sem_destroy(&S0);
   sem_destroy(&S1);
   // sem_destroy(&S2);
   return 0;
}


/* 
S1 = 0, S2 = 1
P1:
   while (1) {
      wait(S1);
      //CS
      signal(S2);
   }
P2: 
   while (1) {
      wait(S2);
      //CS
      signal(S1);
   }

*/