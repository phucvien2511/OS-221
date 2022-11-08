#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define MAX_LOG_LENGTH 10
#define MAX_BUFFER_SLOT 5
#define MAX_LOOPS 50

char logbuf[MAX_BUFFER_SLOT][MAX_LOG_LENGTH];
pthread_mutex_t mtx;
sem_t semFull;
sem_t semEmpty;
int count = 0;
void flushlog();

struct _args
{
   unsigned int interval;
};

void *wrlog(void *data)
{
   /* If buffer is full --> wait */
   sem_wait(&semEmpty);

   pthread_mutex_lock(&mtx);
   char str[MAX_LOG_LENGTH];
   int id = *(int*) data;
   usleep(20);
   sprintf(str, "%d", id);
   strcpy(logbuf[count], str);
   count = (count + 1); /* Only increase count to size MAX_BUFFER_SLOT*/
   // printf("wrlog(): %d %d\n", id, count);
   pthread_mutex_unlock(&mtx);

   /* If buffer is full --> flush */
   if (count == MAX_BUFFER_SLOT) sem_post(&semFull);
   return 0;
}

void flushlog()
{
   int i;
   char nullval[MAX_LOG_LENGTH];

   /* Wait until buffer is full */
   sem_wait(&semFull);
   printf("flushlog() %d\n", count);
   sprintf(nullval, "%d", -1);
   for (i = 0; i < count; i++)
   {
      printf("Slot  %i: %s - %d\n", i, logbuf[i], count);
      strcpy(logbuf[i], nullval);
   }

   fflush(stdout);

   /*Reset buffer */
   
   /* post until count = 0 */
   while (count != 0) {
      sem_post(&semEmpty);
      count--;
   }
   return;

}

void *timer_start(void *args)
{
   while (1)
   {
      flushlog();
      /*Waiting until the next timeout */
      usleep(((struct _args *) args)->interval);
   }
}

int main()
{
   int i;
   count = 0;
   pthread_t tid[MAX_LOOPS];
   pthread_t lgrid;
   int id[MAX_LOOPS];
   pthread_mutex_init(&mtx, NULL);
   sem_init(&semEmpty, 0, MAX_BUFFER_SLOT);
   sem_init(&semFull, 0, 0);
   struct _args args;
   args.interval = 500e3;
   /*500 msec ~ 500 * 1000 usec */

   /*Setup periodically invoke flushlog() */
   pthread_create(&lgrid, NULL, &timer_start, (void*) &args);

   /*Asynchronous invoke task writelog */
   for (i = 0; i < MAX_LOOPS; i++)
   {
      id[i] = i;
      pthread_create(&tid[i], NULL, wrlog, (void*) &id[i]);
   }

   for (i = 0; i < MAX_LOOPS; i++)
      pthread_join(tid[i], NULL);

   sleep(5);
   pthread_mutex_destroy(&mtx);
   sem_destroy(&semEmpty);
   sem_destroy(&semFull);
   return 0;
}
