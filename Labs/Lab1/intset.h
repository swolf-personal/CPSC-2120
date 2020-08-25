#ifndef INTSET_H /* Prevent multiple inclusion... */
#define INTSET_H

typedef struct {
  int key;
  Node *prev;
  Node *next;
} Node;

class Intset {

 private: 
  Node *head;

 public:
  Intset();
  ~Intset();
  bool find(int key);
  void insert(int key);
  void remove(int key);
  void print(void);
};

#endif
