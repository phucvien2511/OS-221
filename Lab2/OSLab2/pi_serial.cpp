#include <iostream>
#include <omp.h>
#include <math.h>
#include <random>
#include <time.h>
#include <iomanip>
struct Point {
   double x;
   double y;
   Point(double x, double y): x(x), y(y) {}
   double distanceTo(const Point &other) {
      return sqrt((other.x - x)*(other.x - x) + (other.y - y) * (other.y - y));
   }
};
void calculatePI (double &PI, const int &nPoint) {
   srand(time(0));
   Point O(0, 0);
   int inCirclePoint = 0;
   int i = 0;
   while (i < nPoint) {
      //random point inside square
      double x = -1 + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX / 2));
      double y = -1 + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX / 2));
      Point randP(x, y);
      if (randP.distanceTo(O) <= 1) {
         inCirclePoint++;
      }
      i++;
   }
   PI = double(inCirclePoint) * 4 / nPoint;
}
int main(int argc, char* argv[]) {
   if (argc < 2) {
      std::cout << "Please enter a number of points!";
   }
   else if (argc > 2) {
      std::cout << "Too many arguments!";
   }
   else {
      int nPoints = std::atoi(argv[1]);
      double PI = 0;
      // clock_t bT, eT;
      double obT = omp_get_wtime();
      // bT = clock();
      calculatePI(PI, nPoints);
      // eT = clock();
      double oeT = omp_get_wtime();
      printf("nPoints = %d\nPI = %f\n", nPoints, PI);
      printf("Runtime of serial (omp): %fs\n", (oeT - obT));
      // printf("Runtime of serial (clock): %fs", (double)(eT - bT) / CLOCKS_PER_SEC);
   }
   return 0;
}