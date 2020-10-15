#ifndef STRINGSET_H /* Prevent multiple inclusion... */
#define STRINGSET_H

#include <string>
using namespace std;

struct Node {
  unsigned int key;
  Node *next;
  Node(unsigned int k, Node *n) { key = k; next = n; }
  Node() { key = 0; next = NULL; }
};

class Stringset {

 private: 
  Node **table;  // array of pointers to linked lists
  int size;      // size of table, as currently allocated
  int num_elems; // number of elements stored in the table

 public:
  Stringset();
  ~Stringset();
  bool find(unsigned int key);
  void insert(unsigned int key);
  void remove(unsigned int key);
  void print(void);
};

#endif
