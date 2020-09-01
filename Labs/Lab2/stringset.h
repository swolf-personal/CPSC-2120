#ifndef STRINGSET_H /* Prevent multiple inclusion... */
#define STRINGSET_H

#include <string>
using namespace std;

class Stringset {

 private: 

  struct Node {
    string key;
    Node *next;
    bool control;
    Node(string k, Node *n) : control(false) { key = k; next = n; }
    Node(bool c, Node *n) : control(c), next(n) {}
    Node() : control(false) { key = ""; next = NULL; }
  };

  Node **table;  // array of pointers to linked lists
  int size;      // size of table, as currently allocated
  int num_elems; // number of elements stored in the table
  
 public:
  Stringset();
  ~Stringset();
  bool find(string key);
  void insert(string key);
  void remove(string key);
  void print(void);
};

#endif
