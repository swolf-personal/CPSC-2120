#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <tuple>

using namespace std;

typedef int keytype; // type of data stored in key fields in tree

struct Node {
  keytype key;
  int size; 
  Node *left, *right;
  Node (keytype k) { key = k; size = 1; left = right = NULL; }
};

int get_size(Node *n) { return n == NULL ? 0 : n->size; }
void update_size(Node *n) { n->size = 1 + get_size(n->left) + get_size(n->right); }

void print_inorder(Node *root)
{
  if (root == NULL) return;
  print_inorder(root->left);
  cout << root->key << "\n";
  print_inorder(root->right);
}

Node* rankedInsert(Node* root, keytype k, int r) {
  if(root == NULL) return new Node(k);
  int rank = get_size(root->left);
  if(r <= rank) root->left  = rankedInsert(root->left,  k, r);
  else          root->right = rankedInsert(root->right, k, r - rank - 1);
  update_size(root);
  return root;
}

int get_rand(int N)
{
  return 123456789 % N;
}

int main(int argc, char *argv[])
{
  int N = atoi(argv[1]);
  if (N<1 || N>100000) {
    cout << "Invalid input size!\n";
    return 0;
  }

  Node *root = NULL;
  int i = N, size = 1;
  while(i > 0)
    root = rankedInsert(root, i--, get_rand(size++));

  cout << N << endl;
  print_inorder(root);
  
  return 0;
}
