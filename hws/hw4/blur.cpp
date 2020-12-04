#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <tuple>
#include "graphics.h"
using namespace std;

/* Image code borrowed from our USA map demo... */

struct Pixel {
  unsigned char r, g, b;
  Pixel(): r(0), b(0), g(0) {}
  Pixel(unsigned char c) : r(c), b(c), g(c) {}
  Pixel(unsigned char rI, unsigned char gI, unsigned char bI) : r(rI), b(bI), g(gI) {};
};

int width, height;
Pixel *image;
int *pixelDepth;
bool *visted;

vector<pair<int, int>> whitePixels;

Pixel white(255);
Pixel black(0);

Pixel &get_pixel(int x, int y)
{
  return image[y*width + x];
}

int &get_pixel_depth(int x, int y)
{
  return pixelDepth[y*width + x];
}

bool& have_visited(int x, int y) {
  return visted[y*width + x];
}

void read_image(const char *filename)
{
  FILE *fp = fopen (filename, "r");
  int dummy = fscanf (fp, "P6\n%d %d\n255%*c", &width, &height);
  image = new Pixel[width * height];
  pixelDepth = new int[width*height];
  for(int i = 0; i < width*height; i++) {
    pixelDepth[i] =1000;
  }
  visted = new bool[width * height];
  dummy = fread (image, width*height, sizeof(Pixel), fp);
  fclose (fp);
}

void write_image(const char *filename)
{
  FILE *fp = fopen (filename, "w");
  fprintf (fp, "P6\n%d %d\n255\n", width, height);
  int dummy = fwrite (image, width*height, sizeof(Pixel), fp);
  fclose (fp);
}

bool operator== (Pixel &a, Pixel &b) {  
  return a.r == b.r && a.g == b.g && a.b == b.b;
}
bool operator!= (Pixel &a, Pixel &b) {  
  return !(a==b);
}

/* To be completed from here on... */

queue<tuple<int,int,int> > worklist;
void calculate_blur()
{  
  while (!worklist.empty()) {    
    int x = get<0>(worklist.front());
    int y = get<1>(worklist.front());
    int d = get<2>(worklist.front());
    unsigned char color = 255*pow(0.9, d);
    worklist.pop();    

    if (d > 80) continue; //At this depth there will be little effect
    if (x<0 || y<0 || x>=width || y>=height) continue;   
    if(get_pixel(x,y).r > color) continue; 
    if (have_visited(x,y)) continue;
    if (get_pixel_depth(x,y) < d) continue;
    have_visited(x,y) = true; 
    get_pixel_depth(x,y) = d;

    get_pixel(x,y) = Pixel(color);

    worklist.push(make_tuple(x+1,y,d+1));    
    worklist.push(make_tuple(x-1,y,d+1));    
    worklist.push(make_tuple(x,y+1,d+1));    
    worklist.push(make_tuple(x,y-1,d+1));
  }

  for(int i = 0; i < width*height; i++) {
    have_visited((i % width),(i / width)) = false;
  }
}

void queue_white() {
  for(int i = 0; i < width*height; i++) {
    if(image[i] == white)
      worklist.push(make_tuple((i % width),(i / width),0));
  }
}

int main(void)
{
  read_image("paw.ppm");

  queue_white();
  calculate_blur();

  write_image("paw2.ppm");
}




