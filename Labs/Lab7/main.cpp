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

//Check for collisions at the current x,y
// NOTE: y (board[x]) needs to be externally inc
bool check_col(int rDex) {
  for (int x = 0; x < rDex; x++) { //Row above
    int x1 = rDex, x2 = x;
    int y1 = board[rDex], y2 = board[x];
    double slope;
    if((y2 - y1) != 0) slope = abs(((double)x2-x1)/(y2-y1));
    else return true;
    if(slope == 1) return true;
  }
  return false;
}

int check_row(int rDex) {
  if(rDex >= boardSize) { cout << "BAD" << endl ; return 0;} //Out of bounds. Nothing to see here...
  int solCount = 0;
  if(rDex == boardSize-1) { //Bottom layer. Logic will return 1 if pass
    for(int y = 0; y < boardSize; y++) { //Iterate over all collums on this row
      board[rDex] = y; //Set the collum to the local check
      if(!check_col(rDex)) { //Valid position
        solCount++;
      }
    }
  } else if (rDex == 0) { 
    if((boardSize % 2) == 0){ //Basic use of symetry... not elegant but I waited too long to optimize
      for(int y = 0; y < (boardSize/2); y++) { //Iterate over all collums on this row
        board[0] = y; //Set the collum to the local check
        if(!check_col(rDex)) { //Valid position
          solCount += check_row(1)*2;
        }
      }
    } else { //Very simple full pts (hopefully) use of sym
      for(int y = 0; y < (boardSize); y++) { //Iterate over all collums on this row
        board[0] = y; //Set the collum to the local check
        if(!check_col(rDex)) { //Valid position
          solCount += check_row(1);
        }
      }
    }
  } else { //Inner layers. Some logic
    for(int y = 0; y < boardSize; y++) { //Iterate over all collums on this row
      board[rDex] = y; //Set the collum to the local check
      if(!check_col(rDex)) { //Valid position
        if(rDex < boardSize-1) solCount += check_row(rDex+1);
      }
    }
  }

  return solCount;
}

int main(int argc, char **argv) {
  assert(atoi(argv[1]) != 0);
  boardSize = atoi(argv[1]);

  board = new int[boardSize];
  for(int i = 0; i < boardSize; i++) board[i] = 0;

  cout << "Total Solutions: " << check_row(0) << endl;

  delete[] board;

  return 0;
}