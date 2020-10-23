#include <iostream>
#include <math.h>
using namespace std;

/*
- Should be a queen on ever row/col
- Recursion
- 2D Array
- 1D Array which gives index of queen on the col

- Try placing a queen at each spot in row per row
-  Recurse on next row to see if solution conflicts, prune asap

- Queens in upper left * 2 = left + right (for row 0)
-   Aka only check halfish:tm: the board (if even)
-   If it's odd, panic
-    If odd, skip middle

N = 14
Solutions = 365,596
*/

int* board;
int boardSize;

int check_row(int rDex) {
  if(rDex >= boardSize) return 0; //???
  int rowSol = 0;
  
  for(int i = 0; i < boardSize; i++) { //Itr - cols
    for (int x = 0; x <= rDex; x++) { //Itr - above
      //x = row
      //y = col
      int x1 = rDex, y1 = board[i]; //??
      int x2 = x, y2 = board[x];
      if(abs(y2-y1) == 1 && (x2-x1 == 0 || abs(x2-x1) == 1)) {
        break;
      } else {
        rowSol += check_row(rDex+1);
      }
    }

    return rowSol;
  }
}

int main() {
  //cin >> boardSize;
  boardSize = 4;

  board = new int[boardSize];

  cout << "Total Solutions: " << check_row(0) << endl;

  delete[] board;

  return 0;
}