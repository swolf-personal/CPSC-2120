#ifndef INTSET_H /* Prevent multiple inclusion... */
#define INTSET_H

struct Node {
  int key;
  Node *next = NULL;
  Node *bfor = NULL;
  Node(int k, Node *n) : key(k), next(n) {}
  Node(int k, Node *n, Node *b) : key(k), next(n), bfor(b) {}
};

class Intset {

 private: 
  Node *head;
  Node *tail;

 public:
  Intset();
  ~Intset();
  bool find(int key);
  void insert(int key);
  void remove(int key);
  void print(void);
};

#endif
