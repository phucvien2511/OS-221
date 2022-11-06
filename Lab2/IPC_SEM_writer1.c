#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#define SHM_KEY 0x123
#define SNAME "/semname"
struct shared_data
{
   int counter;
   int writerID;
};
int main(int argc, char *argv[])
{
   int shmid;
   struct shared_data *data;
   sem_t *sem = sem_open(SNAME, O_CREAT, 0644);
   sem_init(sem, 0, 1);
   shmid = shmget(SHM_KEY, 1000, 0644 | IPC_CREAT);
   if (shmid < 0)
   {
      perror("Shared memory");
      return 1;
   }
   else
   {
      printf("shared memory:  %d\n", shmid);
   }
   if (sem == SEM_FAILED)
   {
      printf("Sem  f ail e d \n");
      return -1;
   }
   data = (struct shared_data *)shmat(shmid, 0, 0);
   if (data == (struct shared_data *)-1)
   {
      perror("shmat");
      exit(1);
   }
   data->counter = 0;
   data->writerID = 1;
   for (int i = 0; i < 20; i++)
   {
      sem_wait(sem);
      printf("Read from Writer ID:  %d with counter :  %d\n",
             data->writerID, data->counter);
      data->writerID = 1;
      data->counter++;
      sem_post(sem);
      sleep(1);
   }
   if (shmdt(data) == -1)
   {
      perror("shmdt");
      return 1;
   }
   if (shmctl(shmid, IPC_RMID, 0) == -1)
   {
      perror("shmctl");
      return 1;
   }
   // Remove the named semaphore
   if (sem_unlink(SNAME) < 0)
      perror("sem_unlink (3) failed ");
   return 0;
}