#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
int count = 0;
int num = 0;
void readFile(int n) {
  FILE *ptr;
  ptr = fopen("numbers.txt", "r");
  while (fscanf(ptr, "%d", &num) != EOF) {
    if (num % n == 0) count++;
  }
  fclose(ptr);
}
int main(int argc, char ** argv) {
  pid_t pid;
  pid = fork();
  if (pid == 0) {
    signal(3, readFile);
    pause();
    printf("%d", count);
    fflush(stdout);
  }
  else if (pid > 0) {
    readFile(2);
    printf("%d", count);
    printf("\n");
    fflush(stdout);
    kill(pid, 3);
    wait(NULL);
  }
  else {
    perror("Fork failed!");
  }  
  return 0;
}
