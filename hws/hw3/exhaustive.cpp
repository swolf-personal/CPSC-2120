#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Candy {
  int weight;
  int taste;
  double value;
  int state;
};

bool operator< (Candy const& lhs, Candy const& rhs) {
  return lhs.value > rhs.value;
}

vector<Candy> candies;
vector<Candy> bestC;

void getCandy() {
  candies.resize(16);
  int weightIn = 0, tasteIn = 0, count = 0;
  //ifstream file("/group/course/cpsc212/f20/lab08/tsp_points.txt");
  ifstream file("candy.txt");
  while(file >> weightIn >> tasteIn) {
    candies[count].weight = weightIn;
    candies[count].taste = tasteIn;
    candies[count].value = (double)tasteIn/weightIn;
    candies[count].state = -1;
    count++;
  }
  file.close();
}

int getBagWeight(int bIndex){
  int total = 0;
  for(Candy c : candies)
    if(c.state == bIndex)
      total += c.weight;
  return total;
}

int tasteTotal(int limit) {
  int total = 0;
  for(int i = 0; i < limit; i++) {
    if(candies[i].state != -1)
      total += candies[i].taste;
  }
  return total;
}

int weightTotal(int limit) {
  int total = 0;
  for(int i = 0; i < limit; i++) {
    if(candies[i].state != -1)
      total += candies[i].weight;
  }
  return total;
}

double getUpperBound(int candyIndex) {
  int preTaste = tasteTotal(candyIndex);
  int preWeight = weightTotal(candyIndex);
  for(int i = candyIndex+1; i < 16; i++) {
    candies[i].state = 0;
  }
  int UB = tasteTotal(16);
  for(int i = candyIndex+1; i < 16; i++) {
    candies[i].state = -1;
  }
  return UB;
}

int incumbent = 0;

void solve(int candyIndex) {
  if(candyIndex == 16) {
    if(tasteTotal(candyIndex) > incumbent){ 
      incumbent = tasteTotal(candyIndex);
      bestC= candies;
      }
    return;
  }
  for(int bagIndex = 2; bagIndex >= -1; bagIndex--) {
    candies[candyIndex].state = bagIndex;
    if(bagIndex != -1) {
      if(getBagWeight(bagIndex) > 2000) continue;
    }
    if(getUpperBound(candyIndex) > incumbent){
      solve(candyIndex+1);
    } else return;
  }
}

int main() {

  getCandy();
  sort(candies.begin(), candies.end());

  solve(0);

  cout << "Exhasutive: " << incumbent << endl;

  return 0;
}