#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
int glb_init_data = 99;
int glb_noinit_data;
void insideProcess() {
   int local_data = 9;
   printf("Process ID: %d\n", getpid());
   printf("Address of process: \n");
   printf("1. glb_init_data: %p\n", &glb_init_data);
   printf("2. glb_noinit_data: %p\n", &glb_noinit_data);
   printf("3. insideProcess(): %p\n", &insideProcess);
   printf("4. local_data: %p\n", &local_data);
}
void func(unsigned long number) {
   unsigned long local_f = number;
   printf("%2lu ---> %p\n", local_f, &local_f);
   local_f--;
   if (local_f > 0) {
      func(local_f);
   }
}
int main() {
   insideProcess();
   return 0;
}