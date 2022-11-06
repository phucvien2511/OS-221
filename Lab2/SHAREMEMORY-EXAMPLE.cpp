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
#include <sys/wait.h>


#define MAX_COLS 5
#define MAX_ROWS 2
#define SNAME "/mysem"


struct Movie {
   int movieID;
};

struct ShareMemory {
   int status;
   Movie movies[MAX_ROWS][MAX_COLS];  
};


enum Status {
   EMPTY, FILLED, FINISH
};


int shmid;

void child_process() {
   int index = 0;
   ShareMemory* shareMemory;
   sem_t* sem = sem_open(SNAME, O_CREAT, 0644);

   shareMemory = (ShareMemory*) shmat(shmid, 0, 0);
   if (shareMemory == (ShareMemory*)(-1)) {
      perror("shmat");
      exit(1);
   }

   shareMemory->status = EMPTY;

   // Change value in shared memory region
   for (int i = 0; i < MAX_ROWS; i++) {
      for (int j = 0; j < MAX_COLS; j++) {
         sem_wait(sem);
         shareMemory->movies[i][j].movieID = i * 10 + j;
         sem_post(sem);
      }
   }

   // Send signal to parent
   shareMemory->status = FILLED;

   // Wait for parent finish processing
   while (shareMemory->status != FINISH) sleep(1);

   if (shmdt(shareMemory) == -1) {
      perror("shmdt");
      exit(1);
   }
   shmctl(shmid, IPC_RMID, NULL);
   sem_close(sem);
}


void parent_process() {
   ShareMemory* shareMemory;

   shareMemory = (ShareMemory*) shmat(shmid, 0, 0);
   if (shareMemory == (ShareMemory*)(-1)) {
      perror("shmat");
      exit(1);
   }

   // Wait for child writing into shared memory region
   while (shareMemory->status != FILLED) sleep(1);

   // Read value from shared memory region
   for (int i = 0; i < MAX_ROWS; i++) {
      printf("From row %d: ", i);
      for (int j = 0; j < MAX_COLS; j++) {
         printf("%d ", shareMemory->movies[i][j].movieID);
      }  
      printf("\n");
   }

   // Send signal to child
   shareMemory->status = FINISH;

   if (shmdt(shareMemory) == -1) {
      perror("shmdt");
      exit(1);
   }
   shmctl(shmid, IPC_RMID, NULL);
}   



int main(int argc, char** argv) {

   sem_t* sem = sem_open(SNAME, O_CREAT, 0644);
   sem_init(sem, 0, 1);
   int size = sizeof(ShareMemory);
   if ((shmid = shmget(IPC_PRIVATE, size, 0644 | IPC_CREAT)) == -1) {
      perror("Share_memory");
      exit(1);
   };

   pid_t first_child = fork();

   if (first_child == 0) {
      child_process();
   } 
   else if (first_child > 0) {
      parent_process();
   }

   return 0;
}