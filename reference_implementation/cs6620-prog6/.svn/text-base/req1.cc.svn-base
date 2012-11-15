
#include "Color.h"
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"
#include "Time.h"
#include "info.h"
#include <iomanip>
#include <iostream>
using std::cerr;
using std::setprecision;
using std::string;

int main(int argc, char** argv)
{
  double t1 = Time::currentSeconds();
  int xres = 512, yres = 512;
  string filename = "image.ppm";
  
  Image image(xres, yres);

  double t2 = Time::currentSeconds();

  Sphere s1(Point(-1, .5, 1), .5);
  Sphere s2(Point(0, 0, 1.1), .8);
  Sphere s3(Point(1, -.5, 1.2), .7);
  for(int i=0;i<yres;i++){
    for(int j=0;j<xres;j++){
      Color result;
      double x = 2 * (j - xres/2. + 0.5)/xres;
      double y = 2 * (i - yres/2. + 0.5)/yres;
      Ray ray(Point(0,0,-1), Vector(x, y, 1));
      if(s1.intersects(ray))
        result = Color(1, .2, .2);
      else if(s2.intersects(ray))
        result = Color(.2, 1, .2);
      else if(s3.intersects(ray))
        result = Color(.2, .2, 1);
      else
        result = Color(.2, .2, .2);
      image.set(j, i, result);
    }
  }

  double t3 = Time::currentSeconds();
  image.write(filename);
  double t4 = Time::currentSeconds();
  cerr << info;
  cerr << "Setup/load time:\t" << setprecision(3) << t2-t1 << " seconds\n";
  cerr << "Render time:\t\t" << setprecision(3) << t3-t2 << " seconds\n";
  cerr << "Post-process time:\t" << setprecision(3) << t4-t3 << " seconds\n";
  return 0;
}
