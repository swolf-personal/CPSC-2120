#include <iostream>
#include <string>
#include <assert.h>

#include "stringset_lab2.h"
//The linker broke :( nope actually just my brain

using namespace std;

// Guaranteed to output a hash value in the range 0 ... 999,999
unsigned int mygivenhash(int x)
{
  return ((unsigned)x * 123147 + 293871) % 100 + 900000;
}

int main(void)
{
  //Oh neat my inset broke... must have been the cobwebs :,(
  //Stringset but acutally intset
  Stringset I; //Use hash to store collisions
  int totalCollistions = 0; //Total times bad hash func collided
  for(int i = 0; i < 1000000; i++) {
    unsigned int col = mygivenhash(i);
    if(!(I.find(col))) I.insert(col);
    else totalCollistions++;
  }

  //Given time i'd have an int-int[] set for each hash
  //The key would be a hash and the value would be an array of numbers that collided
  //Then loop through the set and count the amount of elements. avg and max
  cout << "Total Cols: " << totalCollistions << endl;
}