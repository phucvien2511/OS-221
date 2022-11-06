#include <omp.h>
#include <iostream>
#include <pthread.h>
#include <math.h>
#include <random>
#include <time.h>
#include <sys/time.h>

#define NUM_THREADS 5
int sumInCirclePoint = 0;

struct Point {
   double x;
   double y;
   Point(double x, double y): x(x), y(y) {}
   double distanceTo(const Point &other) {
      return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
   }
};
struct pointThread {
   pthread_t tid;
   int point_hit;
   int nPoints;
   int thread_n;
};
void seed_rand(int thread_n, struct drand48_data *buffer) {
    struct timeval timeVal;

    gettimeofday(&timeVal, NULL);
    srand48_r(timeVal.tv_sec * thread_n + timeVal.tv_usec, buffer);
}

void *calculatePI (void *data) {
   struct drand48_data drand_buffer;
   int hit = 0;
   pointThread *threadInfo = (pointThread *)data;
   seed_rand(threadInfo->thread_n, &drand_buffer);
   Point O(0, 0);
   int i = 0;
   int nPoints = threadInfo->nPoints;
   while (i < nPoints) {
      //random point inside square
      double x, y;
      drand48_r(&drand_buffer, &x);
      drand48_r(&drand_buffer, &y);
      x = x * 2 - 1;
      y = y * 2 - 1;
      Point randP(x, y);
      if (randP.distanceTo(O) <= 1) {
         hit++;
      }
      i++;
   }
   // std:: cout << "inCircle_Local: " << inCircle_Local << ", nPoints: " << nPoints << "\n";
   threadInfo->point_hit = hit;
   pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
   if (argc < 2) {
      fprintf(stderr, "Please enter a number of points!");
      return -1;
   }
   else if (argc > 2) {
      fprintf(stderr, "Too many arguments!");
      return -1;
   }
   int NUM_POINTS = std::atoi(argv[1]);
   if (NUM_POINTS % NUM_THREADS != 0) {
      printf("Please enter a number divisible by NUM_THREADS = %d!\n", NUM_THREADS);
      NUM_POINTS = NUM_POINTS * NUM_THREADS / NUM_THREADS;
   }

   pointThread *datas = new pointThread[NUM_THREADS];
   double obT = omp_get_wtime();
   for (int i = 0; i < NUM_THREADS; i++) {
      datas[i].nPoints = NUM_POINTS / NUM_THREADS;
      datas[i].thread_n = i;
      datas[i].point_hit = 0;
      if (pthread_create(&datas[i].tid, NULL, calculatePI, &datas[i])) {
         printf("Error when creating thread\n");
      }
   }
   for (int i = 0; i < NUM_THREADS; i++) {
      if (pthread_join(datas[i].tid, NULL)) {
         printf("Error while waiting for thread to join\n");
      }
      sumInCirclePoint += datas[i].point_hit;
   }

   double PI = (double)sumInCirclePoint * 4 / NUM_POINTS;

   double oeT = omp_get_wtime();
   printf("nPoints = %d\nPI = %f\n", NUM_POINTS, PI);

   printf("Runtime of multithreads (%d threads): %fs", NUM_THREADS, (oeT - obT));

   /* Clean up and exit */
   delete[] datas;
   pthread_exit(NULL);
   return 0;
}