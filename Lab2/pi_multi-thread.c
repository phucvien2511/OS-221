#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#define NUM_THREADS 5

int numPIC=0; //num point in circle

double randomInterval_01()
{
  double num = ((double)rand()) / ((double)(RAND_MAX));
  return num * 2 + (-1); // num*(high-low)+low;
}

bool createPoint()
{
  double x = randomInterval_01();
  double y = randomInterval_01();
  double dis = sqrt(pow(x,2) + pow(y,2));
  //printf("%lf | %lf\n",x,y);
  return dis <= 1.0;
}
void *counting(void *numPIT) /* threads call this function */
{
	
	long i,num = (long)numPIT;
	for (i= 0; i <num ;i++)
	{	
		srand((unsigned int)time(NULL)+i);
  		if (createPoint())
      		{
      			numPIC++;
      		}
  	}

  	pthread_exit(NULL);
}
int main(int argc, char *argv[])
{
	clock_t t;
 	t=clock();
 	
	if (argc != 2){
		fprintf(stderr,"usage : a.out <integer value>\n") ;
		return -1;
	}
	if (atoi(argv[1])< 0) {
		fprintf(stderr,"%d must be >= 0\n",atoi(argv[1]));
		return -1;
	}
	
	pthread_t threads[NUM_THREADS];
	int nPoints =atoi(argv[1]);
	
	/* create the thread */
	for (long t=0; t<NUM_THREADS; t++){
		//printf ("In main :  creating thread %ld\n" , t ) ;//OK
		long numPIT=((long)nPoints)/NUM_THREADS;//num point in thread
		int rc = pthread_create(&threads[t],NULL,counting,
		(void*) numPIT);
		pthread_join(threads[t], NULL);
		
		if (rc){
		    printf("ERROR; return from pthread_create() is %d\n",rc);
		    exit(-1);
		}

	}
	
	printf("pi= %lf\n", 4*((double)numPIC)/nPoints);
	double time_taken = ((double)t)/CLOCKS_PER_SEC;
  	printf("the program took %f seconds to execute \n", time_taken);
	pthread_exit(NULL);
}

/* The thread will begin control in this function */
