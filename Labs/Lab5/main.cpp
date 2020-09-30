#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<double, double> cut;

int main(void) {
  vector<cut> V;

  double angle1 = 0, angle2 = 0;
  while(cin >> angle1 >> angle2) {
    if(angle1 < angle2) V.push_back(make_pair(angle1, angle2));
    else V.push_back(make_pair(angle2, angle1));
  }

  int N = 0, P = 0;
  for(cut C1 : V){
    for(cut C2 : V) {
      if((C1 != C2) && ((C1.first < C2.first) && (C1.second < C2.second) && (C2.first < C1.second))) {
        P++;
      }
    }
    N++;
  }
  int answer = 1 + N + P;
  cout << "Cuts: " << N << " Intersections: " << P << endl;
  cout << "Answer (Hopefully right : 415199866): " << answer << endl;
}