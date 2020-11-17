#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <string>
using namespace std;

//Put words into hash
//Diif words, check hash (Is there a faster way?)
//Queue diffs as nbrs

typedef string Node;
vector<Node> all_nodes;
map<Node, int> dist;
map<Node, vector<Node>> nbrs;
map<Node, Node> pred;

//DEFINITELY not stolen
void bfs(Node source)
{
  for (Node &a : all_nodes) dist[a] = all_nodes.size();
  dist[source] = 0;
  queue<Node> to_visit;
  to_visit.push(source);

  while (!to_visit.empty()) {
    Node x = to_visit.front();
    to_visit.pop();
    for (Node n : nbrs[x])
      if (dist[n] == all_nodes.size()) {
	      dist[n] = 1 + dist[x];
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
int pathSize = 0;
// Print path from x to y
void mark_path(Node x, Node y)
{
  pathSize++;
  if (x != y) mark_path(x, pred[y]);
  else return;
  cout << y << endl;
}

void build_graph() {
  ifstream input("words5.txt");

  string word;
  while (input >> word)
    all_nodes.push_back(word);

  //This is reallyyyyy slow so... hashtable. Duh.
  for(Node& n : all_nodes) {
    for(Node& w : all_nodes) {
      if(slowDiff(n,w)) nbrs[n].push_back(w);
    }
    pred[n] = n; //This... yeah idk.
  }
  input.close();
}

int main(void) {

  build_graph();
  bfs("graph");
  cout << "graph" << endl; //Cheap hack to not have to fix the recursion
  mark_path("graph", "paths");

  cout << "Path size: " << pathSize << endl;

  return 0;
}