#include <iostream>
#include <string>
#include <assert.h>
#include "stringset_lab2.h"

using namespace std;

/* Return a hash for the string s in the range 0..table_size-1 */
int myhash(unsigned int s, int table_size)
{
  unsigned int i, h = 0;
  h = (2917 + s) % table_size;
  return h;
}

/* Allocate a table of pointers to nodes, all initialized to NULL */
Node **allocate_table(int size)
{
  Node **table = new Node *[size];
  for (int i=0; i<size; i++)
    table[i] = NULL;
  return table;
}

Stringset::Stringset()
{
  size = 4; // initial size of table    
  table = allocate_table(size);
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
bool Stringset::find(unsigned int key)
{
  int h = myhash(key, size);
  Node *n = table[h];
  while (n != NULL) {
    if (n->key == key) return true;
    n = n->next;
  }
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Stringset::insert(unsigned int key)
{
  assert (!find(key));
  num_elems++;

  if (num_elems == size) {
    Node **old_table = table;
    size *= 2;
    table = allocate_table(size);
    
    // Transfer all elements from old_table into table
    for (int i=0; i<size/2; i++) {
      Node *n = old_table[i];
      while (n != NULL) {
	int h = myhash(n->key, size);
	table[h] = new Node(n->key, table[h]);
	Node *to_delete = n;
	n = n->next;
	delete to_delete;
      }
    }

    // De-allocate old_table
    delete [] old_table;
  }

  int h = myhash(key, size);
  table[h] = new Node(key, table[h]);
}

/* Removes a key.  It is an error if key isn't in the set */
void Stringset::remove(unsigned int key)
{
  assert (find(key));
  num_elems--;

  int h = myhash(key, size);
  if (table[h]->key == key) {
    // Delete first node
    Node *to_delete = table[h];
    table[h] = table[h]->next;
    delete to_delete;
  } else {
    // Delete from rest of list
    Node *n = table[h];
    while (n->next->key != key) n = n->next;
    Node *to_delete = n->next;
    n->next = to_delete->next;
    delete to_delete;
  }
}

void Stringset::print(void)
{
  for (int i=0; i<size; i++) {
    Node *n = table[i];
    while (n != NULL) {
      cout << n->key << "\n";
      n = n->next;
    }
  }
}
