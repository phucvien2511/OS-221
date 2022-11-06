#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREAD 6

void* circle_point(void *param);
pthread_t tid[NUM_THREAD] = {0};
int count[NUM_THREAD] = {0};
static long int total_point = 100000000;

static long int count_circle = 0;
int main(int argc, char const *argv[]){
	total_point /= NUM_THREAD;
	static int i;
   clock_t bT, eT;
   bT = clock();
	for(i = 0; i < NUM_THREAD; i++){
		pthread_create(&tid[i], NULL, circle_point, &count[i]);
	}
	for(int i = 0; i < NUM_THREAD; i++){
		pthread_join(tid[i], NULL);
		count_circle += count[i];
	}
   eT = clock();
	double pi = 4.0 * (double)count_circle/(double)total_point/(double)NUM_THREAD;
	printf("PI = %f\n", pi);
   printf("Runtime: %fs", (double) (eT - bT) / CLOCKS_PER_SEC);
	return 0;
}


void* circle_point(void *param){
	int *pcount = (int*)param;
	int i;
	for(i = 0; i < total_point; i++){
		double x = (double)rand()/(double)RAND_MAX;
		double y = (double)rand()/(double)RAND_MAX;
		double r = sqrt(x * x + y * y);
		if(r <= 1) *pcount = *pcount + 1;
	}
	pthread_exit(0);
}