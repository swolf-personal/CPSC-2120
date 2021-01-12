#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <unordered_set>
#include <string>
using namespace std;

//1. Make a map of ints and when they appear
//2. Use std sort w/ custom sort function
//3. ???
//4. Profit

map<int, int> intMap; //Number, Appearence
vector<pair<int, int > > sortingThingy;

bool customCheck(const pair<int, int> lhs, const pair<int, int> rhs) {
  if(lhs.second > rhs.second) return true;
  else if(lhs.first < rhs.first) return true;
  else return false;
}

int main() {
  int num;
  while(cin >> num) {
    if(intMap[num] <= 0) intMap[num] = 1;
    else intMap[num] += 1;
  }

  for (std::map<int, int>::iterator it = intMap.begin(); it != intMap.end(); ++it)
    sortingThingy.push_back(make_pair(it->first, it->second)); //Maps are only sorted on key :( bye map you served me well

  sort(sortingThingy.begin(), sortingThingy.end(), customCheck);
  for(pair<int,int> i : sortingThingy)
    cout << i.first << ", ";

  cout << endl;
}