#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <string>
using namespace std;

typedef string Node;
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

//Returns true if nodes differ by only one character
bool slowDiff(Node a, Node b){
  bool isDiff = false;
  for(int i = 0; i < 5; i++) {
    if(a[i] != b[i]) {
      if(isDiff) return false;
      else isDiff = true;
    }
  }
  return isDiff;
}

void log_path(Node x, Node y)
{
  if (x != y) log_path(x, pred[y]);
  else return;
  longestPath.push_back(y);
}

void build_graph() {
  ifstream input("words5.txt");

  string word;
  while (input >> word)
    all_nodes.push_back(word);

  for(Node& n : all_nodes) {
    for(Node& w : all_nodes) {
      if(slowDiff(n,w)) nbrs[n].push_back(w);
    }
    pred[n] = n;
  }
  input.close();
}

int main(void) {
  build_graph();
  for(Node& n : all_nodes) {
    bfs(n);
    if(maxDist > longestDist) { 
      longestDist = maxDist; 
      longestPath.clear();
      longestPath.push_back(n);
      log_path(n, longestNbrs[0]);
    }
    maxDist = 0;
  }

  for(Node& w : longestPath) {
    cout << w << endl;
  }
  cout << "Longest ladder is " << longestDist+1 << " words." << endl;

  return 0;
}