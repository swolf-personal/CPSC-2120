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
  size = 4; // initial size of table    
  table = new Node *[size]; // allocate table, initialize head ptrs all to NULL
  for (int i=0; i<size; i++) 
    table[i] = NULL;
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
    if (n->key == key) return true;
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
    // TBD: Expand table -- allocate new table of twice the size,
    // re-insert all keys into new table, and de-allocate old table.
    // (you may want to wait and add this code last, once everything
    // else is working, since the class will still function properly,
    // albeit slowly, without this part)
  }

  // TBD: Insert new element
}

/* Removes a key.  It is an error if key isn't in the set */
void Stringset::remove(string key)
{
  assert (find(key));
  num_elems--;

  // TBD: Delete element
}

void Stringset::print(void)
{
  // TBD: Print contents of table
}
