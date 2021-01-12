#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <queue>
#include "glut.h"
#include "graphics.h"
using namespace std;

/* Image code borrowed from our USA map demo... */

struct Pixel {
  unsigned char r, g, b;
};

typedef pair<int, int> Node;
typedef pair<double,Node> pin; // (distance to node, node)
double inf = 999999.0;

vector<Node> all_nodes;
map<Node, double> dist;
map<Node, Node> pred;
map<Node, vector<Node>> nbrs;
map<pair<Node,Node>, int> edge_wt;
map<Node, double> weight;

int width, height;
Pixel *image;
Node megaNode = make_pair(-1, -1);
int best_val = inf;
Node incumbent;

Pixel white = { 255, 255, 255 };
Pixel black = { 0, 0, 0 };
Pixel red = { 255, 0, 0 };


bool operator== (Pixel &a, Pixel &b) {
  return a.r==b.r && a.g==b.g && a.b==b.b;
}


bool operator!= (Pixel &a, Pixel &b) {
  return !(a==b);
}


Pixel &get_pixel(int x, int y) {
  return image[y*width + x];
}


void read_image(const char *filename)
{
  FILE *fp = fopen (filename, "r");
  int dummy = fscanf (fp, "P6\n%d %d\n255%*c", &width, &height);
  image = new Pixel[width * height];
  dummy = fread (image, width*height, sizeof(Pixel), fp);
}


double math(int a) {
    double result = (255 * pow(0.9, a));
    return result;
}

void mark_path(Node x) {
    if (x.second != 0) {
        mark_path(pred[x]);
    }
    get_pixel(x.first, x.second) = red;
}


void build_graph(int numrows, int numcols) {
    int di[] = {+1,-1, 0, 0};
    int dj[] = {0, 0, +1,-1};
    for (int i = 1; i < numcols - 1; i++) {
        for (int j = 1; j < numrows - 1; j++) {
            Node x = make_pair(i, j);
            for (int k = 0; k < 4; k++) {
                Node nbr = make_pair(i + di[k], j + dj[k]);
                nbrs[x].push_back(nbr);
            }
            if (image[(j * numcols) + i].r == 255 &&
                image[(j * numcols) + i].g == 255 &&
                image[(j * numcols) + i].b == 255) {
                    nbrs[megaNode].push_back(x);
            }
        }
    }
    for (int i = 0; i < numcols; i++) {
        for (int j = 0; j < numrows; j++) {
            Node x = make_pair(i, j);
            all_nodes.push_back(x);
        }
    }
}


void build_seam_graph(int numcols, int numrows) {
    nbrs.clear();
    for (int i = 0; i < numcols; i++) {
        for (int j = 0; j < numrows; j++) {
            Node x = make_pair(i, j);
            Node nbr = make_pair(i, j + 1);
            nbrs[x].push_back(nbr);
            if (i < width) {
                Node newNbr = make_pair(i + 1, j + 1);
                nbrs[x].push_back(newNbr);
            }
            if (i > 0) {
                Node newNbr = make_pair(i - 1, j + 1);
                nbrs[x].push_back(newNbr);
            }
            dist[x] = inf;
        }
    }
    for (int i = 0; i < numcols; i++) {
        dist[make_pair(i,0)] = 0;
    }
}


//Node = a pixel
void bfs(Node source) {
    // Use something larger than the max possible sp length...

    for (Node &a : all_nodes) {dist[a] = inf;}
    dist[source] = 0;
    queue<Node> to_visit;
    to_visit.push(source);

    while (!to_visit.empty()) {
        Node x = to_visit.front();
        to_visit.pop();

        for (Node n : nbrs[x]) {
            // Edge weight always just 1
            if (dist[n] == inf) {
                dist[n] = dist[x] + 1;
                pred[n] = x;
                to_visit.push(n);
            }
        }
    }
}

//Node = a pixel
void seam_dijkstra(Node source) {
    // Use something larger than the max possible sp length...
    /*
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
         weight[make_pair(i, j)] = width * height; //Warning: Unused
        }
    }
    */
     priority_queue<pin, vector<pin>, greater<pin>> seam_to_visit;
     seam_to_visit.push(make_pair(0, source));

     while (!seam_to_visit.empty()) {
        Node x = seam_to_visit.top().second;
        seam_to_visit.pop();

        for (Node n : nbrs[x]) {
            int w = get_pixel(n.first, n.second).r;
            if (dist[x] + w < dist[n]) {
                dist[n] = dist[x] + w;
                pred[n] = x;
                seam_to_visit.push(make_pair(dist[n], n));
            }
            //WEIGHT is inspected here, but above (159) is set to the same large value
            //and never revisited
            //if (x.second == height - 1 && weight[x] < best_val) {
            if (dist[x] < best_val) {
                cout << 1 << endl;
                incumbent = x;
                best_val = weight[incumbent];
            }
        }
    }
}


// Import this from your solution to the first part...
void calculate_blur(void) {
    // cout << "megaNode: " << megaNode.first << megaNode.second << endl;
    bfs(megaNode);


    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            get_pixel(i, j).r = math(dist[make_pair(i, j)] - 1);
            get_pixel(i, j).g = math(dist[make_pair(i, j)] - 1);
            get_pixel(i, j).b = math(dist[make_pair(i, j)] - 1);
        }
    }
}


// To be written -- solve a shortest path problem to find a seam and color it red
void calculate_seam(void) {
    // cout << "megaNode: " << megaNode.first << megaNode.second << endl;
    //seam_dijkstra(megaNode);
    seam_dijkstra(make_pair(0,0));
    cout << "Best val: " << best_val << endl;

    //So, here... here the issue seems to be that you basically
    // set every pixel to red, so that would be why they're red.
    /*
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            get_pixel(i, j).r = 255;
            get_pixel(i, j).g = 0;
            get_pixel(i, j).b = 0;
        }
    }
    */
}


/* Code to find and remove seams */
bool seam_exists(void)
{
  for (int x=0; x<width; x++) if (get_pixel(x,0) == red) return true;
  return false;
}


void remove_seam(void) {
cout << "Removing seam to decrease width to " << width-1 << "\n";
    for (int y=0; y<height; y++) {
        int where_red = -1;
        for (int x=0; x<width; x++) {
            if (get_pixel(x,y) == red) {
                if (where_red!=-1) {
                    cout << "Error: row " << y << " has >1 red pixel set\n"; exit(0);
                }
                else {
                    where_red = x;
                }
                if (where_red == -1) {
                    cout << "Error: row " << y << " has 0 red pixels set\n"; exit(0);
                }
                for (int x=where_red; x<width-1; x++) {
                    get_pixel(x,y) = get_pixel(x+1,y);
                }
            }
        }
    }

// Create a new image with one smaller width
    Pixel *orig_image = image;
    width--;
    image = new Pixel[height * width];
    for (int x=0; x<width; x++)
        for (int y=0; y<height; y++)
            get_pixel(x,y) = orig_image[y*(width+1)+x];

    delete [] orig_image;

    calculate_blur();
}


/* Simple 2D interactive graphics code... */
bool show_blurred_image = false;


// Called on each keypress
void keyhandler(int key) {
    if (key == 'q') exit(0);

    // Toggle showing blurred image (off-white pixels)
    if (key == 'b') show_blurred_image = !show_blurred_image;

    // Find and remove one seam
    if (key == 's' && !seam_exists()) calculate_seam();

    // Find and remove 2...9 seams
    if ((key >= '2' && key <= '9') && !seam_exists()) {
        for (int i=0; i<key-'0'; i++) {
            calculate_seam(); remove_seam();
        }
    }
}

// Called any time the scene needs to be re-rendered
void render(void) {
  // Always plot white and red pixels.  Show all other pixels if blur
  // mode is toggled on
    for (int x=0; x<width; x++) {
        for (int y=0; y<height; y++) {
            Pixel p = get_pixel(x, y);
            if (show_blurred_image || p == white || p == red) {
                set_color (p.r, p.g, p.b);
                draw_pixel(x, y);
            }
        }
    }

  // Fill in empty space at right from shrinking the image with blue
    set_color(0, 0, 255);
    for (int x=width; x<800; x++) {
        draw_line (x,0,x,height-1);
    }
}


// Called periodically (msec is amount of time since last called)
// Return true if a re-render is needed
// Right now this function waits a second after finding a seam to
// remove it (allowing it to be visualized briefly)
bool timer(int msec) {
    static int cumulative = 0;
    static bool found_seam = false;
    cumulative += msec;
    if (cumulative > 1000) {
        cumulative -= 1000;
        if (seam_exists()) {
            if (found_seam) {
                remove_seam();
                found_seam = false;
                return true;
            }
            found_seam = true;
        }
    }
    return false;
}


int main(int argc, char *argv[])
{
  read_image("billboard.ppm");
  //build_seam_graph(width, height);
  //calculate_seam();
  //calculate_blur();
  init_graphics(argc, argv, width, height, render, keyhandler, timer);
}
