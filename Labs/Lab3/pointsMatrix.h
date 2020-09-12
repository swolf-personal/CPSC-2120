#ifndef PTSMTX_H
#define PTSMTX_H

using namespace std;

class PointsMatrix {
    private:
        //A node which represents a point
        struct Node {
            double x, y;
            Node *n;
            Node() : x(0), y(0), n(0) {}
            Node(double xIn, double yIn, Node *nIn) : x(xIn), y(yIn), n(nIn) {}
        };
        //A 2D array of nodes
        Node ***points;
        //The amount of "buckets" that will store points
        int b;
    public:
        //Populates class from points.txt w/ size input
        PointsMatrix(int);
        //No leaks hopefully...
        ~PointsMatrix();

        //Give the bucket location of X
        int cacheX(double);
        //Give the bucket location of Y
        int cacheY(double);
        //Put a node in at the right point
        void insert(double, double);
        //Get the distance between two nodes
        double getDist(Node*, Node*);
        //Find the two points which are nearest and return their distance
        double findNearest();

};

#endif