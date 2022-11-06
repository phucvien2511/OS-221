#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* for getopt */

#include <errno.h>
#include <limits.h>

#include "utils.h"

int validate_and_split_argarray (int arraysize, int num_thread, struct _range* thread_idx_range);
int generate_array_data (int* buf, int arraysize, int seednum);

/** validate the array size argument.
 *  the size must be splitable "num_thread".
 */
int validate_and_split_argarray(int arraysize, int num_thread,
                                struct _range* thread_idx_range) {
	
   int i;
   int n;    /* elements per group */

   if(arraysize/num_thread < THRSL_MIN)
	   return -1;

   n = (arraysize + num_thread - 1) / num_thread;    /* elements per group */
	
   for (i = 0; i < num_thread; i++) {
       thread_idx_range[i].start = i*n;
	   thread_idx_range[i].end = ((i + 1)*n - 1 < arraysize) ? (i + 1)*n - 1 : arraysize - 1;
   }

   fprintf(stderr,"\n");
   for (i = 0; i < num_thread; i++) {
       fprintf(stderr,"[%d,%d]\t",thread_idx_range[i].start, thread_idx_range[i].end);
   }
   fprintf(stderr,"\n");
   
   return 0;
}

/** generate "arraysize" data for the array "buf"
 *  validate the array size argument.
 *  the size must be splitable "num_thread".
 */
int generate_array_data(int* buf, int arraysize, int seednum) {
	int i;

	srand(seednum);
	for (i = 0; i < arraysize; i++)
		buf[i] = rand() % (UPBND_DATA_VAL - LWBND_DATA_VAL + 1) + LWBND_DATA_VAL;
	
   fprintf(stderr,"\n[");
   fflush(stderr);
   for (i = 0; i < arraysize; i++) {
       fprintf(stderr,"%3d, \t",buf[i]);
   }
   fflush(stderr);
   fprintf(stderr,"]\n");	
}


/** process string to number.
 *  string is stored in 'nptr' char array.
 *  'num' is returned the valid integer number.
 *  return 0 valid number stored in num 
 *        -1 invalid and num is useless value.
 */
int tonum (const char * nptr, int * num) {
  char * endptr = NULL;
  long number;

  * num = 0;

  /* reset errno to 0 before call */
  errno = 0;

  number = strtol(nptr, & endptr, 0);

  /* test return to number and errno values */
  if (nptr == endptr) {
    fprintf(stderr, " number : %lu  invalid  (no digits found, 0 returned)\n", number);
    return -1;
  } else if (errno == ERANGE && number == LONG_MIN) {
    fprintf(stderr, " number : %lu  invalid  (underflow occurred)\n", number);
    return -1;
  } else if (errno == ERANGE && number == LONG_MAX) {
    fprintf(stderr, " number : %lu  invalid  (overflow occurred)\n", number);
    return -1;
  } else if (errno == EINVAL) {
    /* not in all c99 implementations - gcc OK */
    fprintf(stderr, " number : %lu  invalid  (base contains unsupported value)\n", number);
    return -1;
  } else if (errno != 0 && number == 0) {
    fprintf(stderr, " number : %lu  invalid  (unspecified error occurred)\n", number);
    return -1;
  } else if (errno == 0 && nptr && ! * endptr) {
    fprintf(stderr, " number : %lu    valid  (and represents all characters read)\n", number);
  } else if (errno == 0 && nptr && * endptr != 0) {
    fprintf(stderr, " number : %lu    valid  (but additional characters remain)\n", number);
    return -1;
  }

  if (number < INT_MIN || number > INT_MAX)
    return -1; /* Out of range of INTEGER value */
  else
    *num = * num + (int) number;

  return 0;
}

/** process command line argument.
 *  values are made available through the 'conf' struct.
 *  using the parsed conf to get arguments: the arrsz, tnum, and seednum
 */
int processopts(int argc, char ** argv, struct _appconf * conf) {

   if (tonum(argv[1], & conf->arrsz) < 0) {
      printf("ERROR: invalid value of the argument %s", ARG1);
      help(EXIT_SUCCESS);
   }

   if (tonum(argv[2], & conf->tnum) < 0) {
      printf("ERROR: invalid value of the argument %s", ARG2);
      help(EXIT_SUCCESS);
   }

   if (argc == 4 && tonum(argv[3], & conf->seednum) < 0)
      help(EXIT_SUCCESS);
   else if (argc == 3) /* the leftover of argc==4 and tonum SUCCESS we do nothing*/
      conf->seednum = SEEDNO;

   return 0;
}


/** display help */
void help(int xcode) {
  xcode = xcode ? xcode : 0;

  printf("\n %s, version %s\n\n"
    "  usage:  %s arrsz tnum [seednum]\n\n"
    "  Generate randomly integer array size %s and calculate sum parallelly \n"
    "  using %s threads. The optional %s value use to control the\n"
    "  randomization of the generated array.\n\n"
    "    Arguments:\n\n"
    "      arrsz    specifies the size of array.\n"
    "      tnum     number of parallel threads.\n"
    "      seednum  initialize the state of the randomized generator.\n",
    PACKAGE, VERSION, PACKAGE,
    ARG1, ARG2, ARG3);

  exit(xcode);
}
