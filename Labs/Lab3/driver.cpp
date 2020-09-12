#include <string>
#include <fstream>
#include <iostream>

#include "pointsMatrix.h"

using namespace std;

int main(void)
{
    /*
    So it seems like 2000 "buckets" yielding 500 object linked-lists
    is fastest. I think this is because the bucket loops have a slightly
    lower runtime cost than the linked-list walking loops
    */
    PointsMatrix ptsMtx(2000);

    cout << "Smallest Distance is: " << ptsMtx.findNearest() << endl;

    return 0;
}