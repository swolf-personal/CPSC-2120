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

double get_distance(Point p1, Point p2) {
  return sqrt(pow((p2.x-p1.x), 2) + pow((p2.y-p1.y), 2));
}

double get_total_length(vector<int> tourCheck) {
  double totalDist = 0;
  for(int i = 0; i < 50; i++) {
    if(i == 49) totalDist += get_distance(pts[tourCheck[i]], pts[tourCheck[0]]);
    else totalDist += get_distance(pts[tourCheck[i]], pts[tourCheck[i+1]]);
  }
  return totalDist;
}

void swappening(int i, int randO) {
  int l2 = i, l1 = randO;
  while (l1 < l2) {
    swap(tour[l1], tour[l2]);
    l1++;
    l2--;
  }
}

void reverse() {
  for(int i = 1; i < 49; i++) {
    int j = rand()%i;
    double dist1 = get_distance(pts[tour[i]], pts[tour[i+1]]) + get_distance(pts[tour[j-1]], pts[tour[j]]);
    double dist2 = get_distance(pts[tour[i]], pts[tour[j-1]]) + get_distance(pts[tour[i+1]], pts[tour[j]]);
    if(dist1 > dist2) swappening(i, j);
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

  for(int i = 0; i < 17; i++) {
    random_shuffle(tour.begin(), tour.end());
    for(int cItr = 0; cItr < 1000; cItr++) {
      reverse();
      if(get_total_length(tour) < get_total_length(bestTour)) bestTour = tour;
    }
  }

  cout << "The best tour: " << get_total_length(bestTour) << endl;
  cout << endl;
  for(int i : bestTour) cout << i << " ";
  cout << endl;

  return 0;
}