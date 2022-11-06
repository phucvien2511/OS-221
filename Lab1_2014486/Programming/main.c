//main.c
#include<stdio.h>
#include"readline.h"
#include"factorial.h"
int main (int argc, char * argv[]) {
  char str[50];
  if (read_line(str) == 1) {
    int num = atoi(str);
    num < 0 ? printf("-%d", factorial(-num)) : printf("%d", factorial(num));
  }
  else {
    printf("%d",-1);
  }
  return 0;
}
