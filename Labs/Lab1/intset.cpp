#include <iostream>
#include <assert.h>
#include "intset.h"

using namespace std;

Intset::Intset()
{
  size = 0;
  allocated = 10;  /* start with a 10-element array */
  A = new int[10];
}

Intset::~Intset()
{
  delete[] A;
}

/* Return true if key is in the set */
bool Intset::find(int key)
{
  int low = 0, high = size-1, mid;

  /* Binary search for key... */
  while (low <= high) {
    mid = (low + high) / 2;
    if (key == A[mid]) return true;
    if (key > A[mid]) low = mid+1;
    else high = mid-1;
  }
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Intset::insert(int key)
{
  int i;

  assert (!find(key));

  /* Enlarge (by 2x) the underlying array if it isn't large enough... */
  if (size == allocated) {
    allocated *= 2;
    int *newA = new int[allocated];
    for (i=0; i<size; i++) newA[i] = A[i];
    delete[] A;
    A = newA;
  }

  /* Shift up elements in array to make room for new element... */
  size++;
  i = size-1;
  while (i>0 && A[i-1]>key) {
    A[i] = A[i-1];
    i--;
  }
  A[i] = key;
}

/* Removes a key.  It is an error if key isn't in the set */
void Intset::remove(int key)
{
  int i;

  assert (find(key));

  /* Shift down elements in array to plug gap left by deleted element... */
  for (i=size-1; key!=A[i]; i--);
  while (i<size-1) {
    A[i] = A[i+1];
    i++;
  }
  size--;
}

void Intset::print(void)
{
  int i;
  for (i=0; i<size; i++)
    cout << A[i] << "\n";
}
