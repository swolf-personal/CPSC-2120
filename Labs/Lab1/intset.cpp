#include <iostream>
#include <assert.h>
#include "intset.h"

using namespace std;

Intset::Intset()
{
  head = NULL;
}

Intset::~Intset()
{
  //Two itrs
  Node* prevItr = NULL;
  Node* itr = head;
  //If a list is larger than zero
  while(itr->next != NULL)
  {
    prevItr = itr;
    itr = itr->next;
    delete prevItr;
  }
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
      Node *newNode = new Node;
      newNode->key = key;
      newNode->next = itr;
      head = newNode;
    //Head is less than, find the location where the itr becomes greater
    } else if(itr->key < key) {
      do {
        prevItr = itr;
        itr = itr->next;
      } while(itr->key < key);
      Node *newNode = new Node;
      newNode->key = key;
      newNode->next = itr;
      prevItr->next = newNode;
    }
  //No list, make one
  } else {
    Node *newNode = new Node;
    newNode->key = key;
    newNode->next = NULL;
    
    head = newNode;
  }
}

/* Removes a key.  It is an error if key isn't in the set */
void Intset::remove(int key)
{
  assert(find(key));

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
          delete itr;
          break;
          }
        //Not head, not as ez
        else {
          prevItr->next = itr->next;
          delete itr;
          break;
        }
      }
      itr = itr->next;
    }
  }
}

void Intset::print(void)
{
  Node* itr = head;

  if(itr != NULL)
  {
    do {
      cout << itr->key << endl;
      itr = itr->next;
    } while(itr != NULL);
  }
}
