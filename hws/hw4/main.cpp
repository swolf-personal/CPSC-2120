#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <queue>
#include "graphics.h"
using namespace std;

/* Image code borrowed from our USA map demo... */

/* Given Pixel struct
struct Pixel {
  unsigned char r, g, b;
};
*/

//Custom Pixel struct
struct Pixel {
  unsigned char r, g, b;
  Pixel(): r(0), b(0), g(0) {}
  Pixel(unsigned char c) : r(c), b(c), g(c) {}
  Pixel(unsigned char rI, unsigned char gI, unsigned char bI) : r(rI), g(gI), b(bI) {}
};

/* Given variable setup
int width, height;
Pixel *image;

Pixel white = { 255, 255, 255 };
Pixel black = { 0, 0, 0 };
Pixel red = { 255, 0, 0 };
*/

//Custom variable setup
int width, height;
Pixel *image;
bool *visted;

Pixel white(255);
Pixel black(0);
Pixel red(255,0,0);

int *interest;
int *weight;
int *pred;

queue<tuple<int,int,int> > pixelQueue;

bool operator== (Pixel &a, Pixel &b) {
  return a.r==b.r && a.g==b.g && a.b==b.b;
}
bool operator!= (Pixel &a, Pixel &b) {
  return !(a==b);
}

Pixel &get_pixel(int x, int y)
{
  return image[y*width + x];
}

//Somewhat confusingly a higher interest value corresponds to a lower importance... boring is interesting!
int &get_interest(int x, int y)
{
  return interest[y*width + x];
}

int &get_weight(int x, int y)
{
  return weight[y*width + x];
}

int downscale(int x, int y)
{
  return y*width + x;
}

//Simple 1D array of visted pixels. Could use one long int?
bool& have_visited(int x, int y) {
  return visted[y*width + x];
}

void read_image(const char *filename)
{
  FILE *fp = fopen (filename, "r");
  int dummy = fscanf (fp, "P6\n%d %d\n255%*c", &width, &height);
  image = new Pixel[width * height];
  dummy = fread (image, width*height, sizeof(Pixel), fp);
}

//Enqueue all white pixels
void queue_white() {
  for(int i = 0; i < width*height; i++) {
    if(image[i] == white)
      pixelQueue.push(make_tuple((i % width),(i / width),0));
  }
}

// Import this from your solution to the first part...
void calculate_blur(void)
{
  queue_white();
  for(int p = 0; p < width*height; p++)
    visted[p] = false;
  for(int p = 0; p < width*height; p++)
    interest[p] = 0;

  while (!pixelQueue.empty()) {    
    int x = get<0>(pixelQueue.front());
    int y = get<1>(pixelQueue.front());
    int d = get<2>(pixelQueue.front());
    unsigned char color = 255*pow(0.9, d);
    pixelQueue.pop();    

    if (x<0 || y<0 || x>=width || y>=height) continue;   
    if (get_pixel(x,y).r > color) continue; 
    if (have_visited(x,y)) continue;
    
    get_interest(x,y) = d;
    have_visited(x,y) = true; 
    get_pixel(x,y) = Pixel(color);

    pixelQueue.push(make_tuple(x+1,y,d+1));    
    pixelQueue.push(make_tuple(x-1,y,d+1));    
    pixelQueue.push(make_tuple(x,y+1,d+1));    
    pixelQueue.push(make_tuple(x,y-1,d+1));
  }
}

void set_seam() {
  for(int p = 0; p < width*height; p++)
    pred[p] = -1;
  for(int w = 0; w < width*height; w++)
    weight[w] = 99999999;
  
  //Oh look a queue
  queue<int> seamQ;

  //Put all da top dawgs into the queue
  for(int i = 0; i < width; i++) {
    seamQ.push(i);
    weight[i] = 0;
  }
  
  while(!seamQ.empty()) {
    int pixel = seamQ.front();
    int x = (pixel % width);
    int y = (pixel / width);
    seamQ.pop();

    if (x<0 || y<0 || x>=width || y>=height) continue;
    if (y+1>=height) continue; //If no one is below we ain't interested
    for(int i = -1; i <= 1; i++) {
      if(x+i < 0 || x+i >= width) continue;
      if(get_weight(x,y) + get_pixel(x+i,y+1).r < get_weight(x+i, y+1)) {
        get_weight(x+i, y+1) = get_weight(x,y) + get_pixel(x+i,y+1).r;
        pred[downscale(x+i,y+1)] = pixel;
        seamQ.push(downscale(x+i,y+1));
      }
    }
  }
}

// To be written -- solve a shortest path problem to find a seam and color it red
void calculate_seam(void)
{
  set_seam();

  int bestWeight = 999999;
  int currentPred = -1;
  for(int i = 0; i < width; i++) {
    if(weight[downscale(i,height-1)] < bestWeight) {
      bestWeight = weight[downscale(i,height-1)];
      currentPred = downscale(i,height-1);
    }
  }
  while(currentPred != -1) {
    image[currentPred] = red;
    currentPred = pred[currentPred];
  }
}

/* Code to find and remove seams */

bool seam_exists(void)
{
  for (int x=0; x<width; x++) if (get_pixel(x,0) == red) return true;
  return false;
}

void remove_seam(void)
{

  cout << "Removing seam to decrease width to " << width-1 << "\n";
  for (int y=0; y<height; y++) {
    int where_red = -1;
    for (int x=0; x<width; x++)
      if (get_pixel(x,y) == red)
	  if (where_red!=-1) { cout << "Error: row " << y << " hass >1 red pixel set\n"; exit(0); }
	  else where_red = x;
    if (where_red == -1) { cout << "Error: row " << y << " has 0 red pixels set\n"; exit(0); }
    for (int x=where_red; x<width-1; x++) get_pixel(x,y) = get_pixel(x+1,y);
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
void keyhandler(int key)
{
  if (key == 'q') {
    exit(0);
    delete [] pred;
    delete [] weight;
    delete [] visted;
    delete [] interest;
  }

  // Toggle showing blurred image (off-white pixels)
  if (key == 'b') show_blurred_image = !show_blurred_image;

  // Find and remove one seam
  if (key == 's' && !seam_exists()) calculate_seam();

  // Find and remove 2...9 seams
  if ((key >= '2' && key <= '9') && !seam_exists()) 
    for (int i=0; i<key-'0'; i++) { calculate_seam(); remove_seam(); }
}

// Called any time the scene needs to be re-rendered
void render(void)
{
  // Always plot white and red pixels.  Show all other pixels if blur
  // mode is toggled on
  for (int x=0; x<width; x++)
    for (int y=0; y<height; y++) {
      Pixel p = get_pixel(x, y);
      if (show_blurred_image || p == white || p == red) {
	      set_color (p.r, p.g, p.b); 
	      draw_pixel(x, y);
      }
    }

  // Fill in empty space at right from shrinking the image with blue
  set_color(0, 0, 255);
  for (int x=width; x<800; x++)
    draw_line (x,0,x,height-1);
}

// Called periodically (msec is amount of time since last called)
// Return true if a re-render is needed
// Right now this function waits a second after finding a seam to
// remove it (allowing it to be visualized briefly)
bool timer(int msec)
{
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
  pred = new int[width*height];
  weight = new int[width*height];
  visted = new bool[width * height]; //Probably should refactor as local to function, but who cares about leaks now right? :)
  interest = new int[width * height];
  calculate_blur();
  init_graphics(argc, argv, width, height, render, keyhandler, timer);
}
