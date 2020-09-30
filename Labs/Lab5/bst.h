#ifndef BST_H
#define BST_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <tuple>

using namespace std;

class BST {
  private:
    struct Node {
      double key;
      int size;    // number of nodes in this node's subtree
      Node *left;
      Node *right;
      Node (double k) { key = k; size = 1; left = right = NULL; }
    };

    Node* head;

  public:
    Node* insert(Node*, double);
    void print_inorder(Node*);
    Node* find(Node*, double);
    Node* predfind(Node*, double);
    Node* join(Node*, Node*);
    Node* remove(Node*, double);
    pair<Node*, Node*> split(Node*, double);
    Node* insert_keep_balanced(Node*, double);
    Node* getHead();
}

#endif