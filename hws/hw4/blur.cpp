#include <iostream>
#include <cstdio>
#include <cmath>
#include <queue>
#include <tuple>

using namespace std;

/* Image code borrowed from our USA map demo... */

struct Pixel {
  unsigned char r, g, b;
  Pixel(): r(0), b(0), g(0) {}
  Pixel(unsigned char c) : r(c), b(c), g(c) {}
};

int width, height;
Pixel *image;
bool *visted;

Pixel white(255);
Pixel black(0);

queue<tuple<int,int,int> > pixelQueue;

Pixel &get_pixel(int x, int y)
{
  return image[y*width + x];
}

bool& have_visited(int x, int y) {
  return visted[y*width + x];
}

void read_image(const char *filename)
{
  FILE *fp = fopen (filename, "r");
  int dummy = fscanf (fp, "P6\n%d %d\n255%*c", &width, &height);
  image = new Pixel[width * height];
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

/* To be completed from here on... */

bool operator== (Pixel &a, Pixel &b) {  
  return a.r == b.r && a.g == b.g && a.b == b.b;
}

void queue_white() {
  for(int i = 0; i < width*height; i++) {
    if(image[i] == white)
      pixelQueue.push(make_tuple((i % width),(i / width),0));
  }
}

void calculate_blur()
{
  queue_white();

  while (!pixelQueue.empty()) {    
    int x = get<0>(pixelQueue.front());
    int y = get<1>(pixelQueue.front());
    int d = get<2>(pixelQueue.front());
    unsigned char color = 255*pow(0.9, d);
    pixelQueue.pop();    

    if (d > 20) continue; //At this depth there will be little effect...
    if (x<0 || y<0 || x>=width || y>=height) continue;   
    if (get_pixel(x,y).r > color) continue; 
    if (have_visited(x,y)) continue;
    
    have_visited(x,y) = true; 
    get_pixel(x,y) = Pixel(color);

    pixelQueue.push(make_tuple(x+1,y,d+1));    
    pixelQueue.push(make_tuple(x-1,y,d+1));    
    pixelQueue.push(make_tuple(x,y+1,d+1));    
    pixelQueue.push(make_tuple(x,y-1,d+1));
  }
}

int main(void)
{
  read_image("paw.ppm");
  
  calculate_blur();

  write_image("paw2.ppm");
}




