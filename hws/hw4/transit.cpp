#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <string>
using namespace std;

//1 1 1 1 | 1 1 1 1 
//1 1 1 1 | 1 1 1 0
//0 0 0 0 | 0 0 0 1

//Toss a point or two for the poor? 
//This program was a victim of time,
// as are we all...

typedef int Node;
vector<Node> all_nodes;
map<Node, int> dist;
map<Node, vector<Node>> nbrs;
map<Node, Node> pred;

int maxDist = 0;
int longestDist = 0;

vector<Node> longestPath;
vector<Node> longestNbrs;

//DEFINITELY not stolen
void bfs(Node source)
{
  for (Node &a : all_nodes) {
    dist[a] = all_nodes.size();
    pred[a] = a;
  }
  dist[source] = 0;
  queue<Node> to_visit;
  to_visit.push(source);

  while (!to_visit.empty()) {
    Node x = to_visit.front();
    to_visit.pop();
    longestNbrs.clear();
    for (Node n : nbrs[x])
      if (dist[n] == all_nodes.size()) {
	      dist[n] = 1 + dist[x];
        maxDist = 1 + dist[x];
        longestNbrs.push_back(n);
	      pred[n] = x;
	      to_visit.push(n);
      }
  }
}

//nbrs are all possible int state changes from current
void populateNbrs() {

}

int main() {

}