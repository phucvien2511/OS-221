#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdbool.h> 
#include <sys/stat.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <math.h>
#define MAX_COLS 1700
#define MAX_ROWS 3
#define SNAME "/avrgsem"
#define INPUT_1_PATH "movie-100k_1.txt" //include your path here
#define INPUT_2_PATH "movie-100k_2.txt" //include your path here
#define OUTPUT_FINAL "finaloutput.txt" // path of output file
using namespace std;
struct MovieRate {
   int mID;
   int rateCOUNT;
   double rateSUM;
   double AVRG;
   MovieRate(){
      mID = -1;
      rateCOUNT = 0;
      rateSUM = 0;
   }
   MovieRate operator&= (MovieRate &other) {
      this->mID = other.mID;
      this->rateCOUNT = other.rateCOUNT;
      this->rateSUM = other.rateSUM;
      return *this;
   }
   void calcAvrg() {
      this->AVRG = rateSUM / rateCOUNT;
   }
   void print() {
      printf("{ ID: %d, COUNT: %d, SUM: %.2f, AVRG: %.2f}", this->mID, this->rateCOUNT, this->rateSUM, this->AVRG);
   }
   string toString() const {
      stringstream ss;
      ss << this->mID << "\t" << this->AVRG;
      return ss.str();
   }
};

enum Status {
   EMPTY, FILLED, FINISH
};
struct ShareMemory {
   int status_A;
   int status_B;
   int status_PARENT;
   MovieRate movies[MAX_ROWS][MAX_COLS];
};


int shmid;
//File processing
vector<long double> tokenize(string& line) {
   vector<long double> tokens;
   stringstream check(line);
   string token;
   while(getline(check, token, '\t')) {
      long double pattern = stod(token.c_str());
      tokens.push_back(pattern);
   }
   return tokens;
}
void writeFile (string filename, ShareMemory* F, int index) {
   fstream myfile;
   myfile.open(filename, ios::out);
   for (int i = 0; i < MAX_COLS; i++) {
      if (F->movies[index][i].mID != 0) {
         myfile << F->movies[index][i].toString() << "\n";
      }
   }
}
void readFileI (string filename, ShareMemory* &F, int index) {
   ifstream myfile;
   myfile.open(filename, ios::out);
   string line;
   while(std::getline(myfile, line)) { 
      vector<long double> strToInt = tokenize(line);
      int iD = strToInt[1];
      double rate = strToInt[2];
      F->movies[index][iD].mID = iD;
      F->movies[index][iD].rateCOUNT++;
      F->movies[index][iD].rateSUM += rate;
   }
}
void childA_process() {
   ShareMemory* shareMemory;
   sem_t* sem = sem_open(SNAME, O_CREAT, 0644);

   shareMemory = (ShareMemory*) shmat(shmid, 0, 0);
   if (shareMemory == (ShareMemory*)(-1)) {
      perror("shmat");
      exit(1);
   }

   shareMemory->status_A = EMPTY;

   // Change value in shared memory region
   sem_wait(sem);
   readFileI(INPUT_1_PATH, shareMemory, 0);
   sem_post(sem);

   // Send signal to parent
   shareMemory->status_A = FILLED;

   // Wait for parent finish processing
   while (shareMemory->status_PARENT != FINISH) sleep(10);

   if (shmdt(shareMemory) == -1) {
      perror("shmdt");
      exit(1);
   }
   shmctl(shmid, IPC_RMID, NULL);
   sem_close(sem);
}
void childB_process() {
   ShareMemory* shareMemory;
   sem_t* sem = sem_open(SNAME, O_CREAT, 0644);

   shareMemory = (ShareMemory*) shmat(shmid, 0, 0);
   if (shareMemory == (ShareMemory*)(-1)) {
      perror("shmat");
      exit(1);
   }

   shareMemory->status_B = EMPTY;

   // Change value in shared memory region
   sem_wait(sem);
   readFileI(INPUT_2_PATH, shareMemory, 1);
   sem_post(sem);
   
   // Send signal to parent
   shareMemory->status_B = FILLED;

   // Wait for parent finish processing
   while (shareMemory->status_PARENT != FINISH) sleep(1);

   if (shmdt(shareMemory) == -1) {
      perror("shmdt");
      exit(1);
   }
   shmctl(shmid, IPC_RMID, NULL);
   sem_close(sem);
}

void parent_process() {
   ShareMemory* shareMemory;

   shareMemory = (ShareMemory*) shmat(shmid, 0, 0);
   if (shareMemory == (ShareMemory*)(-1)) {
      perror("shmat");
      exit(1);
   }

   // Wait for child writing into shared memory region
   while (shareMemory->status_A != FILLED && shareMemory->status_B != FILLED) sleep(3);

   // Read value from shared memory region
   printf("From childA and file 1: \n");
   for (int i = 0; i < MAX_COLS; i++) {
      if (shareMemory->movies[0][i].mID != 0) {
         shareMemory->movies[0][i].calcAvrg();
         // writeFile("res_1.txt", shareMemory, 0);
      }
   }
   printf("From childB and file 2: \n");
   for (int i = 0; i < MAX_COLS; i++) {
      if (shareMemory->movies[1][i].mID != 0) {
         shareMemory->movies[1][i].calcAvrg();
         // writeFile("res_2.txt", shareMemory, 1);
      }
   }
   for (int i = 0; i < MAX_COLS; i++) {
      if (shareMemory->movies[0][i].mID != 0 || shareMemory->movies[1][i].mID != 0) {
         shareMemory->movies[2][i].mID = shareMemory->movies[0][i].mID;
         shareMemory->movies[2][i].rateCOUNT = shareMemory->movies[0][i].rateCOUNT + shareMemory->movies[1][i].rateCOUNT;
         shareMemory->movies[2][i].rateSUM = shareMemory->movies[0][i].rateSUM + shareMemory->movies[1][i].rateSUM;
         shareMemory->movies[2][i].calcAvrg();
      }
   }
   writeFile(OUTPUT_FINAL, shareMemory, 2);
   // Send signal to child
   shareMemory->status_PARENT = FINISH;

   if (shmdt(shareMemory) == -1) {
      perror("shmdt");
      exit(1);
   }
   shmctl(shmid, IPC_RMID, NULL);
} 


int main() {
   sem_t* sem = sem_open(SNAME, O_CREAT, 0644);
   sem_init(sem, 0, 1);
   int size = sizeof(ShareMemory);
   if ((shmid = shmget(IPC_PRIVATE, size, 0644 | IPC_CREAT)) == -1) {
      perror("Share_memory");
      exit(1);
   };

   // childA_process();
   pid_t childA, childB;
   childA = fork();
   if (childA == 0) {
      childA_process();
   } 
   else {
      childB = fork();
      if (childB == 0) {
         //child B
         childB_process();
      }
      else {
         //parent process
         parent_process();
      }
   }
   return 0;
}