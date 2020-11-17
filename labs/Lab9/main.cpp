#include <iostream>

#include <map>
#include <stack>
#include <vector>

using namespace std;

struct Node {int a, b;};

bool pred[1001][1001];

int A = 0, B = 0, X = 0;

void handleInput() {
  cout << "A: ";
  cin >> A;
  cout <<"B: ";
  cin >> B;
  cout << "X: ";
  cin >> X;
  cout << endl;
  return;
}

pair<int,int> handlePourA(int a, int b) {
  int pourAttempt = b+a;
  if(pourAttempt > B) {
    b = B;
    a = a - B;
  }
  return make_pair(a,b);
}

pair<int,int> handlePourB(int a, int b) {
  int pourAttempt = b+a;
  if(pourAttempt > A) {
    a = A;
    b = b - A;
  }
  return make_pair(a,b);
}

bool doTheLab(int a, int b) {
  if(pred[a][b]) return false;
  pred[a][b] = true;
  if(a > 1000 || b > 1000) return false;
  if(a+b == X) return true;
  if(doTheLab(a+A,b)) return true;
  if(doTheLab(a,b+B)) return true;
  if(doTheLab(0,b)) return true;
  if(doTheLab(a,0)) return true;
  if(doTheLab(handlePourA(a,b).first, handlePourA(a,b).second)) return true;
  if(doTheLab(handlePourB(a,b).first, handlePourB(a,b).second)) return true;
  return false;
}

/* 
vector<Node> all_nodes;

map<Node, Node> pred;
map<Node, int> dist;
map<Node, vector<Node>> nbrs;

Node source;

void definitelynotstolen(Node s) {
  for(Node& a : all_nodes) dist[a] = all_nodes.size();
  dist[source] = 0;
  stack<Node> to_vist;
  to_vist.push(source);

  while(!to_vist.empty()) {
    Node x = to_vist.top();
    to_vist.pop();
    for(Node n : nbrs[x])
      if(dist[n] == all_nodes.size()) {
        dist[n] = 1 + dist[x];
        pred[n] = x;
        to_vist.push(n);
      }
  }
}

void build_nbrs() {
  all_nodes.resize(1000*1000);
  for(int a = 0; a <= 1000; a++) {
    for(int b = 0; b <= 1000; b++) {
      all_nodes[a+b].a = a; //bad. very bad.
      all_nodes[a+b].b = b;
    }
  }
  for(Node &n : all_nodes) {
    nbrs[n].
  }
}*/

int main() {

  handleInput();

  if(!doTheLab(0,0)) cout << "Impossible." << endl;
  else cout << "At least it worked..." << endl;

  return 0;
}