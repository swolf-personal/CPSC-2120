#include <iostream>
#include <cstdlib>
using namespace std;

int get_rand(int N)
{
  return 123456789 % N;
}

int main(int argc, char *argv[])
{
  int N = atoi(argv[1]);
  if (N<1 || N>100000) {
    cout << "Invalid input size!\n";
    return 0;
  }
  
  // Generate and print bad input of size N for prog3
  cout << N << endl;
  for (int i=N; i>0; i--){
    cout << "When size is: " << i << " pivot is: " << get_rand(i) << endl;
  }
    
  
  return 0;
}
