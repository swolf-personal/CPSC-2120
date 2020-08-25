#include <iostream>
#include <assert.h>
#include "intset.h"

using namespace std;

Intset::Intset()
{

}

Intset::~Intset()
{

}

/* Return true if key is in the set */
bool Intset::find(int key)
{
  Node* itr = head;
  if(itr != NULL)
  {
    do {
      if(itr->key == key)
        return true;
      if(itr->next != NULL)
        itr = itr->next;
    } while (itr->next != NULL);
  }
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Intset::insert(int key)
{
  assert(!find(key));

  //Two itrs
  Node* prevItr = NULL;
  Node* itr = head;
  //If a list is larger than zero
  if(itr != NULL)
  {
    //If head is greater make new node new head
    if(itr->key > key) {
      Node newNode;
      newNode.key = key;
      newNode.next = itr;
      head = &newNode;
    //Head is less than, find the location where the itr becomes greater
    } else if(itr->key < key) {
      do {
        prevItr = itr;
        itr = itr->next;
      } while(itr->key < key);
      Node newNode;
      newNode.key = key;
      newNode.next = itr;
      prevItr->next = &newNode;
    }
  //No list, make one
  } else {
  Node newNode;
  newNode.key = key;
  head = &newNode;
  }
}

/* Removes a key.  It is an error if key isn't in the set */
void Intset::remove(int key)
{
  assert(!find(key));

  //Two itrs
  Node* prevItr = NULL;
  Node* itr = head;
  //If a list is larger than zero
  if(itr != NULL)
  {
    while(itr != NULL) 
    {
      if(itr->key == key) {
        //If head, ez
        if(prevItr == NULL) {
          head = itr->next;
          break;
          }
        //Not head, not as ez
        else {
          prevItr->next = itr->next;
          break;
        }
      }
      itr = itr->next;
    }
  }
}

void Intset::print(void)
{
  Node *itr = head;
  while(itr != NULL) {
    cout << itr->key << endl;
    itr = itr->next;
  }
}
