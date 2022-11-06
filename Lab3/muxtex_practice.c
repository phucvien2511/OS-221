#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
int MAX_COUNT = 10000000;
static int count = 0;
pthread_mutex_t mutex;
void * f_count(void *sid) {
   int i;
   for (int i = 0; i < MAX_COUNT; i++) {
      pthread_mutex_lock(&mutex);
      count++;
      pthread_mutex_unlock(&mutex);
   }
   printf("Thread %s: holding %d\n", (char*) sid, count);
}
int main() {
   pthread_t thread1, thread2;
   pthread_mutex_init(&mutex, NULL);
   pthread_create(&thread1, NULL, &f_count, (void *)"1");
   pthread_create(&thread2, NULL, &f_count, (void *)"2");
   pthread_join(thread1, NULL);
   pthread_join(thread2, NULL);
   pthread_mutex_destroy(&mutex);
   return 0;
}