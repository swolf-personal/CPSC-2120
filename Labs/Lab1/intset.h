#ifndef INTSET_H /* Prevent multiple inclusion... */
#define INTSET_H

struct Node {
  int key;
  Node *next = NULL;
  Node(int k, Node *n) : key(k), next(n) {}
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
