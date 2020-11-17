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
  return lhs.value < rhs.value;
}

vector<Candy> candies;
vector<Candy> runningBest;

int getBagWeight(int bIndex){
  int total = 0;
  for(Candy c : candies)
    if(c.state == bIndex)
      total += c.weight;
  return total;
}

bool putCandy(int b, Candy& c, bool overfill = false) {
  if(overfill || (getBagWeight(b) + c.weight) <= 2000) {
    c.state = b;
    return true;
  }
  return false;
}

bool pruneBag(int b) {
  int i = 0;
  for(i = 0; i < 16; i++)
    if(candies[i].state != -1) break;
  if(i == 15 && candies[15].state == -1) return false;

  if((getBagWeight(candies[i].state) - candies[i].weight) <= 2000) {
    candies[i].state = -1;
    return false;
  } else {
    candies[i].state = -1;
    return true;
  }
}

bool iterativeAdd() {
  for(Candy c : candies)
    for(int x = 0; x < 3; x++)
      if(c.state == -1)
        if(putCandy(x,c)) return true;

  return false;
}

void getCandy() {
  candies.resize(16);
  int weightIn = 0, tasteIn = 0, count = 0;
  ifstream file("/group/course/cpsc212/f20/lab08/tsp_points.txt");
  //ifstream file("candy.txt");
  while(file >> weightIn >> tasteIn) {
    candies[count].weight = weightIn;
    candies[count].taste = tasteIn;
    candies[count].value = (double)tasteIn/weightIn;
    candies[count].state = -1;
    count++;
  }
  file.close();
}

void fillBags() {
  random_shuffle(candies.begin(), candies.end());

  for(int i = 0; i < 16; i++) {
    putCandy(rand()%3, candies[i], true);
  }
}

void greedyLevel() {
  sort(candies.begin(), candies.end());

  for(int i = 0; i < 3; i++)
    while(getBagWeight(i) > 2000)
      pruneBag(i);
      iterativeAdd();
}

int tasteTotal(int limit) {
  int total = 0;
  for(int i = 0; i < limit; i++) {
    if(candies[i].state != -1)
      total += candies[i].taste;
  }
  return total;
}

void fillyWilly() {
  sort(candies.begin(), candies.end());

  for (int i=0; i<3; i++) 
    for (int j=0; j<16; j++)
      putCandy(i, candies[j]);
}

int lightestBag(int pS) {
  int curWeight = getBagWeight(0);
  int bag = 0;
  for(int i = 1; i <3; i++){
    if(i == pS) continue;
    if(getBagWeight(i) < curWeight) {
      curWeight = getBagWeight(i);
      bag = i;
    }
  }
  return bag;
}

int uuhhhh() {
  int bestValue = 0;
  for(int i = 0; i < 1000; i++) {
    getCandy();
    random_shuffle(candies.begin(), candies.end());
    fillyWilly();

    int piece = 0;
    while(candies[piece].state == -1)
      piece = rand()%16;
    int preState = candies[piece].state;
    candies[piece].state = -1;

    int bag = lightestBag(preState);
    putCandy(bag,candies[piece], true);

    for(int j=0; getBagWeight(bag) > 2000; j++)
      candies[j].state = -1;
    fillyWilly();
    if (tasteTotal(16) > bestValue) {
      bestValue = tasteTotal(16);
    }
  }
  return bestValue;
}

int main() {
  int bent = 0;
  srand(time(NULL));
  bent = uuhhhh();

  cout << getBagWeight(0) << endl;
  cout << getBagWeight(1) << endl;
  cout << getBagWeight(2) << endl;

  cout << bent << endl;

  return 0;
}