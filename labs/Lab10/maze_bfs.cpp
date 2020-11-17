#include <iostream>
#include <vector>
#include <string>
#include "colors.h"
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
using namespace std;

typedef pair<int,int> Node;

vector<string> maze;
Node start, goal;

vector<Node> all_nodes;
map<Node, int> dist;
map<Node, Node> pred;
map<Node, vector<Node>> nbrs;

/*
Not needed but how comparison of pairs works...
bool operator< (Node x, Node y) {
  if (x.first < y.first) return true;
  if (x.first > y.first) return false;
  return x.second < y.second;
}
*/

// Running time is usually linear: O(#nodes + #edges)  "O(n + m)"  "O(V + E)"
// but with using maps (since they have logarithmic access time): O((n + m) log n)
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

void build_graph(void)
{
  int numrows = maze.size();
  int numcols = maze[0].length();

  for (int i=0; i<numrows; i++) 
    for (int j=0; j<numcols; j++) 
      all_nodes.push_back(make_pair(i,j));
  
  int di[] = {+1,-1, 0, 0};
  int dj[] = {0, 0, +1,-1};
  for (int i=1; i<numrows-1; i++) 
    for (int j=1; j<numcols-1; j++) 
      for (int k=0; k<4; k++) {
	Node x = make_pair(i,j);
	Node nbr = make_pair(i + di[k], j + dj[k]);
	if (maze[nbr.first][nbr.second]!='#') 
	  nbrs[x].push_back(nbr);
      }
}

// Print path from x to y
void mark_path(Node x, Node y)
{
  if (x != y) mark_path(x, pred[y]);
  maze[y.first][y.second] = '.';
}

int main(void)
{
  string s;
  while (getline(cin, s)) maze.push_back(s);

  int numrows = maze.size();
  int numcols = maze[0].length();

  for (int i=0; i<numrows; i++) 
    for (int j=0; j<numcols; j++) {
      if (maze[i][j] == 'A') { start = make_pair(i,j); }
      if (maze[i][j] == 'B') { goal = make_pair(i,j); }
    }

  build_graph();
  bfs(start);
  mark_path(start, goal);
  
  for (int i=0; i<maze.size(); i++) {
    for (int j=0; j<maze[i].length(); j++) {
      if (maze[i][j] == '#') cout << color_whitered;
      if (maze[i][j] == ' ') cout << color_white; 
      if (maze[i][j] == '.') cout << color_whiteblue; 
      cout << maze[i][j];
    }
    cout << color_white << "\n";
  }
}
