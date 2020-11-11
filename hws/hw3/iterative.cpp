#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
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
  while(cin >> weightIn >> tasteIn) {
    rawCandy[count].weight = weightIn;
    rawCandy[count].taste = tasteIn;
    rawCandy[count].value = (double)tasteIn/weightIn;
    count++;
  }
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
  bags.resize(4);

  getCandy();
  random_shuffle(rawCandy.begin(), rawCandy.end());
  //sort(rawCandy.begin(), rawCandy.end());

  cout << "Total Value: " << totalValue() << endl;

  return 0;
}