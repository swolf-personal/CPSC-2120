#include <iostream>
#include <string.h>
#include <assert.h>
#include "stringset.h"

using namespace std;

/* Return a hash for the string s in the range 0..table_size-1 */
int myhash(string s, int table_size)
{
  unsigned int i, h = 0;
  for (i=0; i<s.length(); i++)
    h = (h * 2917 + (unsigned int)s[i]) % table_size;
  return h;
}

Stringset::Stringset()
{
  size = 8; // initial size of table    
  table = new Node *[size]; // allocate table, initialize head ptrs all to NULL
  for (int i=0; i<size; i++) {
    //table[i] = NULL;
    Node *tail = new Node(true, NULL);
    table[i] = new Node(true, tail); 
  }
  num_elems = 0; 
}

Stringset::~Stringset()
{
  for (int i=0; i<size; i++) {
    while (table[i] != NULL) {
      Node *temp = table[i];
      table[i] = table[i]->next;
      delete temp;
    }
  }
  delete[] table;
}

/* Return true if key is in the set */
bool Stringset::find(string key)
{
  int h = myhash(key, size);
  Node *n = table[h];
  while (n != NULL) {
    if (n->key == key && !n->control) return true;
    n = n->next;
  }
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Stringset::insert(string key)
{
  assert (!find(key));
  num_elems++;

  if (num_elems == size) {
    Node **thiccTable = new Node *[size*2];
    for (int i=0; i<size*2; i++) 
      thiccTable[i] = (i<size) ? table [i] : new Node(true, new Node(true, NULL));
    delete[] table;
    table = thiccTable;
    size = size * 2;
  }

  // TBD: Insert new element
  int h = myhash(key, size);
  table[h]->next = new Node(key, table[h]->next);
}

/* Removes a key.  It is an error if key isn't in the set */
void Stringset::remove(string key)
{
  assert (find(key));
  num_elems--;

  // TBD: Delete element
  int h = myhash(key, size);
  for(Node *n = table[h]; n != NULL; n = n->next){
    if (n->next != NULL && n->next->key == key) {
      Node *temp = n->next;
      n->next = n->next->next;
      delete temp;
      break;
    }
  }
}

void Stringset::print(void)
{
  // TBD: Print contents of table
  cout << "Contents of the stringset: " << endl;
  for(int i = 0; i < size; i++) {
    for(Node *n = table[i]; n != NULL; n = n->next) {
      if(!n->control)
        cout << n->key << " | ";
    }
  }
  cout << endl;
}
