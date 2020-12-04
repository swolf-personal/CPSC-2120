#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <queue>
#include "graphics.h"
using namespace std;

/* Image code borrowed from our USA map demo... */

struct Pixel {
  unsigned char r, g, b;
};

int width, height;
Pixel *image;

Pixel white = { 255, 255, 255 };
Pixel black = { 0, 0, 0 };
Pixel red = { 255, 0, 0 };

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

void read_image(const char *filename)
{
  FILE *fp = fopen (filename, "r");
  int dummy = fscanf (fp, "P6\n%d %d\n255%*c", &width, &height);
  image = new Pixel[width * height];
  dummy = fread (image, width*height, sizeof(Pixel), fp);
}

// Import this from your solution to the first part...
void calculate_blur(void)
{

}

// To be written -- solve a shortest path problem to find a seam and color it red
void calculate_seam(void)
{

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
  if (key == 'q') exit(0);

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
  calculate_blur();
  init_graphics(argc, argv, width, height, render, keyhandler, timer);
}
