#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;
struct MovieRate {
   int mID;
   int rateCOUNT;
   double rateSUM;
   MovieRate(){
      mID = -1;
      rateCOUNT = 0;
      rateSUM = 0;
   }
   MovieRate operator&= (MovieRate &other) {
      this->mID = other.mID;
      this->rateCOUNT = other.rateCOUNT;
      this->rateSUM = other.rateSUM;
   }
};
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
void writeFile (string filename, MovieRate* &F) {
   fstream myfile;
   myfile.open(filename, ios::out);
   for (int i = 0; i < 2000; i++) {
      if (F[i].mID != -1) {
         myfile << F[i].mID << '\t' << fixed << setprecision(4) << (double)F[i].rateSUM / F[i].rateCOUNT << '\n';
      }
   }
}
void readFileI (string filename, MovieRate* &F) {
   ifstream myfile;
   myfile.open(filename, ios::out);
   string line;
   while(std::getline(myfile, line)) { 
      vector<long double> strToInt = tokenize(line);
      int iD = strToInt[1];
      double rate = strToInt[2];
      F[iD].mID = iD;
      F[iD].rateCOUNT++;
      F[iD].rateSUM += rate;
   }
}
void readFileII (string filename, MovieRate* &F) {
   ifstream myfile;
   myfile.open(filename, ios::out);
   string line;
   while(std::getline(myfile, line)) { 
      vector<long double> strToInt = tokenize(line);
      int iD = strToInt[0];
      double rate = strToInt[1];
      F[iD].mID = iD;
      F[iD].rateCOUNT++;
      F[iD].rateSUM += rate;
   }
}
int main() {
   pid_t childA, childB;
   childA = fork();
   if (childA == 0) {
      MovieRate *F1 = new MovieRate[2000];
      readFileI("movie-100k_1.txt", F1);
      writeFile("F1-res.txt", F1);
   } else {
      childB = fork();

      if (childB == 0) {
         MovieRate *F2 = new MovieRate[2000];
         readFileI("movie-100k_2.txt", F2);
         writeFile("F2-res.txt", F2);
      } 
      else {
         wait(NULL);
         wait(NULL);
         MovieRate *F3 = new MovieRate[2000];
         readFileII("F1-res.txt", F3);
         readFileII("F2-res.txt", F3);
         writeFile("finaloutput.txt", F3);
      }
   } 
   return 0;
}