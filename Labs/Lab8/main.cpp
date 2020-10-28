#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

struct Point {
  double x;
  double y;
};

vector<Point> pts;
vector<int> tour;
vector<int> bestTour;

double getLength(Point p1, Point p2) {
  return sqrt(pow((p2.x-p1.x), 2) + pow((p2.y-p1.y), 2));
}

double tourLength(vector<int> tourCheck) {
  double totalDist = 0;
  for(int i = 0; i < 50; i++) {
    if(i == 49) totalDist += getLength(pts[tourCheck[i]], pts[tourCheck[0]]);
    else totalDist += getLength(pts[tourCheck[i]], pts[tourCheck[i+1]]);
  }
  return totalDist;
}

void swapRun(int i, int j) {
  while (j < i) {
    swap(tour[j], tour[i]);
    j++; i--;
  }
}

void optimize() {
  for(int i = 1; i < 49; i++) {
    int j = rand()%i;
    double dist1 = getLength(pts[tour[i]], pts[tour[i+1]]) + getLength(pts[tour[j-1]], pts[tour[j]]);
    double dist2 = getLength(pts[tour[i]], pts[tour[j-1]]) + getLength(pts[tour[i+1]], pts[tour[j]]);
    if(dist1 > dist2) swapRun(i, j);
  }
}

int main(void) {
  tour.resize(50);
  for(int i = 0; i<50; i++) 
    tour[i] = i;
  bestTour = tour;

  pts.resize(50);
  for(int i = 0; i<50; i++)
    cin >> pts[i].x >> pts[i].y;

  for(int i = 0; i < 17; i++) { //Listen... I'm not sure why but in this case 17 is a really cool number
    random_shuffle(tour.begin(), tour.end());
    for(int cItr = 0; cItr < 1000; cItr++) {
      optimize();
      if(tourLength(tour) < tourLength(bestTour)) bestTour = tour;
    }
  }

  cout << "The best tour: " << tourLength(bestTour) << endl;
  cout << endl;
  for(int i : bestTour) cout << i << " ";
  cout << endl;

  return 0;
}