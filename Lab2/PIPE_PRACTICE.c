#include <stdio.h>
#include <unistd.h>
int main()
{
   int pipefds[2];
   int returnstatus;
   int pid;
   char writemessages[20] = "Hello";
   char readmessage[20];
   returnstatus = pipe(pipefds);
   if (returnstatus == -1)
   {
      printf("Unable to  create pipe\n");
      return 1;
   }
   pid = fork();
   // Child process
   if (pid == 0)
   {
      read(pipefds[0], readmessage, sizeof(readmessage));
      printf("Child Process :  Reading ,  message  is  %s\n", readmessage);
      return 0;
   }
   // Parent process
   printf("Parent Process :  Writing ,  message  is  %s\n", writemessages);
   write(pipefds[1], writemessages, sizeof(writemessages));
   return 0;
}