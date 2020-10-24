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
  if(rDex >= boardSize) return 1;
  int rowSol = 0;
  
  if(rDex == 0) { //No collide checks for top row
    for (int x = 0; x <= boardSize; x++) { //Cols
      rowSol += check_row(rDex+1);
      board[0]++; 
    }
  } else { //Full check loops for all below
    for (int x = 0; x <= boardSize; x++) { //Cols
      for (int x = 0; x <= rDex; x++) { //Row above
        int x1 = x, x2 = rDex;
        int y1 = board[x], y2 = board[rDex];
        double slope;
        if((y2 - y1) != 0) slope = abs(((double)x2-x1)/(y2-y1));
        if((y2 - y1) == 0 || slope == 1) {
          if(board[rDex] < boardSize) board[rDex]++;
          else return 0; //We got through an entire row w/o a valid location, prune dis
          break;
        } else {
          rowSol++;
          rowSol += check_row(rDex+1);
          board[rDex]++;
        }
      }
    }
  }
  //# solutions 0
  // if rDex >= boardSize return 0
  // if rDex == 0
  //   check_row for each pos in board[0]
  // else 
  //   check for collisions
  //   if colide, either move on or prune
  //   else check_row(rDex+1), board[x]++

  return rowSol;
}

int main() {
  //cin >> boardSize;
  boardSize = 4;

  board = new int[boardSize];
  for(int i = 0; i < boardSize; i++) board[i] = 0;

  cout << "Total Solutions: " << check_row(0) << endl;

  delete[] board;

  return 0;
}