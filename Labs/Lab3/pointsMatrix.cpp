#include <iostream>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <fstream>
#include <iostream>

#include "pointsMatrix.h"

using namespace std;

PointsMatrix::PointsMatrix(int b) {
    this->b = b;

    points = new Node**[b]; //The error here is trippin. Nothing is wrong 
    for (int i = 0; i < b; i++)
        points[i] = new Node*[b];

    double x,y;
    ifstream pointsfile("points.txt");
    while (pointsfile >> x >> y)
        insert(x,y);
    pointsfile.close();

    cout << "Points Loaded.\n";
}

PointsMatrix::~PointsMatrix() {
    for (int i = 0; i < b; i++)
        delete [] points[i];
    delete [] points;
}

int PointsMatrix::cacheX(double x) {
    return (int)(x * b);
}

int PointsMatrix::cacheY(double y) {
    return (int)(y * b);
}

void PointsMatrix::insert(double x, double y) {
    points[cacheX(x)][cacheY(y)] = new Node(x,y,points[cacheX(x)][cacheY(y)]);
    return;
}

double PointsMatrix::getDist(Node *n1, Node *n2) {
    return sqrt(pow((n1->x - n2->x),2) + pow((n1->y - n2->y),2));
}

double PointsMatrix::findNearest() {
    double smallestDist = 100;
    for(int x = 0; x < this->b; x++) {
        for (int y = 0; y < this->b; y++) {
            for(Node* n = points[x][y]; n != NULL; n = n->n) {
                for(int i = x-1; i < x+1; i++) {
                    for(int j = y-1; j < y+1; j++) {
                        if(!((i < 0 || j < 0) || (i > b || j > b))) {
                            for(Node* n2 = points[i][j]; n2 != NULL; n2 = n2->n) {
                                if(n == n2)
                                    continue;
                                double newDist = getDist(n, n2);
                                if (newDist < smallestDist)
                                    smallestDist = newDist;
                            } 
                        }
                    }
                }
            }
        }
    }
    return smallestDist;
}
