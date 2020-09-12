#include <iostream>
#include <math.h>
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
  for (int i=0; i<size; i++)
    table[i] = new Node(true, new Node(true, NULL)); 
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

/* FOR O(1) hash func
*  Subtract 2917^x + ASCII code of original char from h where x is the amt of chars back
*  Add 2917^x + int val of char to get new hash
*/

void Stringset::suggestSpellings(string key) {
  int h = myhash(key, size);

  //For each char location; Subtract the relevent amt from the hash
  for(int i = key.length(); i > 0; i--){
    int temp = h - (pow(2917, i) + key[i]);
    //For each possible character substitution add the relevent amt back
    for(char c = 96; c < 122; c++) {
      int temp2 = temp + (pow(2917, i) + c);
      //Quick exit if hash not possible
      if (temp2 > size) continue;
      //If hash is possible print all keys at that hash
      for(Node *n = table[temp2]; n != NULL; n = n->next) {
        if(!n->control)
          cout << n->key << endl; //TODO - Resolve collision?
      }
    }
  }
}

/* Return true if key is in the set */
bool Stringset::find(string key)
{
  for(Node *n = table[myhash(key,size)]; n != NULL; n = n->next)
    if (n->key == key && !n->control) return true;
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Stringset::insert(string key)
{
  assert (!find(key));
  num_elems++;

  if (num_elems == size) {
    Node **newTable = new Node *[size*2];

    for (int i=0; i<size*2; i++)
      newTable[i] = new Node(true, new Node(true, NULL));

    for (int i=0; i<size; i++) {
      for(Node *n = table[i]; n != NULL; n = n->next) {
        if(!n->control) {
          newTable[myhash(n->key, size*2)]->next = n; //Rehash without deleting old mem?
        }
      }
    }

    delete[] table; //drop table...
    table = newTable;
    size *= 2;
  }

  if(0) { //Bad bad no rehash code - TODO - Remove bad code
    Node **thiccTable = new Node *[size*2];
    for (int i=0; i<size*2; i++) 
      thiccTable[i] = (i<size) ? table [i] : new Node(true, new Node(true, NULL));
    delete[] table;
    table = thiccTable;
    size *= 2;
  }

  int h = myhash(key, size);
  table[h]->next = new Node(key, table[h]->next);
}

/* Removes a key.  It is an error if key isn't in the set */
void Stringset::remove(string key)
{
  assert (find(key));
  num_elems--;

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
  cout << "Contents of the stringset: " << endl;
  for(int i = 0; i < size; i++)
    for(Node *n = table[i]; n != NULL; n = n->next)
      if(!n->control)
        cout << n->key << " | ";
  cout << endl;
}
