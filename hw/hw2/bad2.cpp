#include <iostream>
#include <cstdlib>
using namespace std;

//I'll just be taking this... don't mind me prog2
int myHash(int x)
{
  return ((unsigned int)x * 3 + 17) % 10000;
}

int main(int argc, char *argv[])
{
  int N = atoi(argv[1]);
  if (N<1 || N>100000) {
    cout << "Invalid input size!\n";
    return 0;
  }
  
  // Generate and print bad input of size N for prog2
  // Choose a location and unleash a flash mob
  cout << N << endl;
  int i = 0, x = 0;
  while(x < N) {
    if(myHash(i) == 1){ cout << i << endl; x++;}
    i++;
  }

  return 0;
}
