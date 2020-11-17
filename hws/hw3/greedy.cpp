#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

struct Candy {
  int weight;
  int taste;
  double value;
};

struct Bag {
  vector<int> candies;
  int weight;
};

bool operator < (Candy const& lhs, Candy const& rhs) {
  return lhs.value > rhs.value;
}

vector<Bag> bags;
vector<Candy> rawCandy;

void getCandy() {
  rawCandy.resize(16);
  int weightIn = 0, tasteIn = 0, count = 0;
  //ifstream file("/group/course/cpsc212/f20/lab08/tsp_points.txt");
  ifstream file("candy.txt");
  while(file >> weightIn >> tasteIn) {
    rawCandy[count].weight = weightIn;
    rawCandy[count].taste = tasteIn;
    rawCandy[count].value = (double)tasteIn/weightIn;
    count++;
  }
  file.close();
}


int totalValue() {
  int total = 0;
  for(Bag bag : bags) {
    for(int c : bag.candies) {
      total += rawCandy[c].taste;
    }
  }
  return total;
}

int main() {
  bags.resize(3);

  getCandy();
  sort(rawCandy.begin(), rawCandy.end());

  vector<int> inBag;
  for(int b = 0; b < 3; b++) {
    for(int c = 0; c < 16; c++) {
      if(find(inBag.begin(), inBag.end(), c) != inBag.end()) continue;
      if((bags[b].weight + rawCandy[c].weight) >= 2000) continue;
      bags[b].candies.push_back(c);
      bags[b].weight += rawCandy[c].weight;
      inBag.push_back(c);
    }
  }

  cout << "Total Value: " << totalValue() << endl;

  return 0;
}