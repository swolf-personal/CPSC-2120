#include <iostream>
#include <assert.h>
#include "intset.h"

using namespace std;

Intset::Intset() {
  head = NULL;
}

Intset::~Intset() {
  //Two itrs
  Node* pNode = NULL;
  Node* cNode = head;
  //If a list is larger than zero
  while(cNode != NULL) {
    pNode = cNode;
    cNode = cNode->next;
    delete pNode;
  }
}

/* Return true if key is in the set */
bool Intset::find(int key) {
  Node* cNode = head;
  if(cNode != NULL) {
    do {
      if(cNode->key == key)
        return true;
      cNode = cNode->next;
    } while (cNode != NULL);
  }
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Intset::insert(int key) {
  assert(!find(key));

  if(head == NULL){head = new Node(key, head); return;}
  for(Node* cNode = head; cNode != NULL; cNode = cNode->next) {
    if(key > cNode->key)
    {
      head = cNode->next = new Node(key, cNode->next);
    }
    //if (head == NULL || cNode->key > key && ) {
    //  cNode->next = new Node(key, cNode->next);
    //} else if (cNode->key < key && cNode->next == NULL ||  cNode->key < key && cNode->next->key > key) {
  }
}

  //if(head == NULL){head = tail = new Node(key, NULL); return;} 
  //for(Node* cNode = head; cNode != NULL; cNode = cNode->next) {
    //if (cNode->key < key && cNode->next == NULL ||  cNode->key < key && cNode->next->key > key) {tail->next = new Node(key, tail); tail = tail->next;}
    //if (cNode->key < key && cNode->next == NULL ||  cNode->key < key && cNode->next->key > key) {cNode->next = new Node(key, cNode->next);}
  //}

//if (cNode->key < key && cNode->next == NULL || cNode->next->key > key) {cNode->next = new Node(key, cNode->next);}

  /*
  //Two itrs
  Node* pNode = NULL;
  Node* cNode = head;
  //If a list is larger than zero
  if(cNode != NULL) {
    //If head is greater make new node new head
    if(cNode->key > key) {
      head = new Node(key, cNode);
    //Head is less than, find the location where the cNode becomes greater
    } else if(cNode->key < key) {
      do {
        pNode = cNode;
        cNode = cNode->next;
      } while(cNode->key < key);

      Node *newNode = new Node(key, cNode);;
      pNode->next = newNode;
    }
  //No list, make one
  } else {
    head = new Node(key, NULL);
  }
  
}*/

/* Removes a key.  It is an error if key isn't in the set */
void Intset::remove(int key) {
  assert(find(key));

  //Two itrs
  Node* pNode = head;
  Node* cNode = head;

  //List exists
  while(cNode != NULL) {
    if(cNode->key == key) {
        pNode->next = cNode->next;
        if(cNode == head) head = NULL;
        delete cNode;
        break;
    }
    pNode = cNode;
    cNode = cNode->next;
  }
}

void Intset::print(void) {
  Node* cNode = head;

  if(cNode != NULL) {
    do {
      cout << cNode->key << endl;
      cNode = cNode->next;
    } while(cNode != NULL);
  }
}
