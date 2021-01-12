#include <iostream>
using namespace std;

int main() {
  int *i = new int;

  *i = 0;

  cout << *i << endl;

  delete i;

  i = new int;

  *i = 1;

  cout << *i << endl;

  delete i;

  return 0;
}