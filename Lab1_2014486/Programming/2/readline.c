#include "readline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int read_line(char* str) {
  size_t size;
  char *S = NULL;
  getline(&S, &size, stdin);
  size_t len = strlen(S);
  if (S[len - 1] == '\n') S[len - 1] = '\0';
  len = strlen(S);
  strcpy(str, S);
  for (size_t i = 0; i < len; i++) {
     if (i == 0 && S[i] == '-') continue; 
     if (!isdigit(S[i])) return 0;
  }
  return 1;
}

