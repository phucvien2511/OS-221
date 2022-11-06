#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistdio.h>
#include <unistd.h>
#define MAX_ITEMS 1
#define LOOPS 1 * MAX_ITEMS
#define THREADS 1
sem_t semFull;
sem_t semEmpty;
/* initialize buffer */
int buffer[MAX_ITEMS];
int fill = 0;
int use = 0;
// int count = 0;
void put(int data); /* put data into buffer */
int get(); /* get data from buffer */

void *producer(void* arg) {
   int tid = *(int*) arg;
   for (int i = 0; ; i++) {
      //TODO
      sem_wait(&semEmpty);
         int x = rand() % 100;
         put(x);
         printf("Producer %d put data %d\n", tid, x);
      sem_post(&semFull);
      sleep(1);
      //TODO
   }
   pthread_exit(NULL);
}
void *consumer(void* arg) {
   int tmp = 0;
   int tid = *(int*) arg;
   while (tmp != -1) {
      //TODO - 
      sem_wait(&semFull);
         tmp = get();
         printf("Consumer %d get data %d\n", tid, tmp);
      sleep(1);
      sem_post(&semEmpty);
      //TODO - 
   }
   pthread_exit(NULL);
}
int main() {
   srand(time(NULL));
   int tid[THREADS];
   pthread_t producers[THREADS];
   pthread_t consumers[THREADS];
   sem_init(&semEmpty, 0, 0);
   sem_init(&semFull, 0, 1);
   for (int i = 0; i < THREADS; i++) {
      pthread_create(&producers[i], NULL, &producer, & tid[i]);
      pthread_create(&consumers[i], NULL, &consumer, & tid[i]);
   }

   for (int i = 0; i < THREADS; i++) {
      pthread_join(producers[i], NULL);
      pthread_join(consumers[i], NULL);
   }
   sem_destroy(&semEmpty);
   sem_destroy(&semFull);
   return 0;
}
void put(int data) {
   buffer[fill] = data;
   fill = (fill + 1) % MAX_ITEMS;
}
int get() {
   int tmp = buffer[use];
   use = (use + 1) % MAX_ITEMS;
   return tmp; 
}