#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



#define MSG_SIZE 20

static int pipefd1[2], pipefd2[2];

void INIT(void) {
   if (pipe(pipefd1) < 0 || pipe(pipefd2) < 0) {
      perror("pipe");
      exit(EXIT_FAILURE);
   }
}

void WRITE_TO_PARENT(void) {
   /* send parent a message through pipe */
   // TODO
   char stuId[MSG_SIZE] = "2014486";
   if (write(pipefd2[1], &stuId, sizeof(stuId)) == -1) {
      perror("error with write from child process (fd2)!");
      return;
   }
   printf("Child_send_message_to_parent (through pipefd2)!\n");
   return;
}

void READ_FROM_PARENT(void) {
   /* read message sent by parent from pipe */
   char msg[MSG_SIZE];
   // TODO
   if (read(pipefd1[0], &msg, sizeof(msg)) == -1) {
      perror("error with read from child process (fd1)!");
      return;
   }
   printf("Child_receive_message_from_parent (pipefd1): %s\n", msg);
   return;
}

void WRITE_TO_CHILD(void) {
   /* send child a message through pipe */
   // TODO
   char msg[MSG_SIZE] = "CSE - HCMUT";
   if (write(pipefd1[1], &msg, sizeof(msg)) == -1) {
      perror("error with write from parent process !");
      return;
   }
   printf("Parent_send_message_to_child (through pipefd1)!\n");
   return;
}

void READ_FROM_CHILD(void) {
   /* read message sent by child from pipe */
   char msg[MSG_SIZE];
   // TODO
   if (read(pipefd2[0], &msg, sizeof(msg)) == -1) {
      perror("error with read from child process (fd2)!");
      return;
   }
   printf("Parent_receive_message_from_child (pipefd2): %s\n", msg);
   return;
}

int main(int argc, char** argv) {
   INIT();
   pid_t pid;
   pid = fork();

   // set a timer, process will end after 1 second.
   alarm(10);
   if (pid != 0) {
      // Parent process
      while (1) {
         sleep(rand() % 2 + 1);
         WRITE_TO_CHILD();
         READ_FROM_CHILD();
         printf("\n");
      }

   } else {
      // Child process
      while (1) {
         sleep(rand() % 2 + 1);
         WRITE_TO_PARENT();
         READ_FROM_PARENT();
         printf("\n");
      }
   }

   return 0;
}