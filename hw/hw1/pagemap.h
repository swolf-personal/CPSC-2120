#ifndef PAGEMAP_H /* Prevent multiple inclusion... */
#define PAGEMAP_H

#include "stringintmap.h"

#include <string>

using namespace std;

class PageMap {

 private: 

  struct Webpage {
    string url;
    int numLinks;
    int numWords;
    vector<Webpage> links;
    StringIntMap words;
  };

  struct Node {
    string key;
    int val;
    Webpage page;
    Node *next;
    Node(string k, int v, Node *n) { key = k; val = v; next = n; }
    Node(string k, Webpage wp, Node *n) { key = k; page = wp; next = n; }
  };

  Node **table;  // array of pointers to linked lists
  int size;      // size of table, as currently allocated
  int num_elems; // number of elements stored in the table
  Node **allocate_table(int size);  // private helper function
  int myhash(string, int);
  
 public:
  PageMap();
  ~PageMap();
  Webpage &operator[](string key);
  bool find(string key);
  void insert(string key, Webpage val);
  void remove(string key);
  void print(void);
  int get_num_elems(void);
  string *get_keys(void);

  void link(Webpage, Webpage);
};

#endif
