
#ifndef Image_h
#define Image_h

#include "Color.h"
#include <string>

struct Pixel {
  char r, g, b;
};

class Image {
 public:
  Image(int xres, int yres);
  ~Image();
  void set(int x, int y, const Color& c) {
    Pixel p;
    p.r = c.r() < 0.f? 0: c.r() >= 1.f? 255 : (int)(c.r() * 255.f);
    p.g = c.g() < 0.f? 0: c.g() >= 1.f? 255 : (int)(c.g() * 255.f);
    p.b = c.b() < 0.f? 0: c.b() >= 1.f? 255 : (int)(c.b() * 255.f);
    data[y][x] = p;
  }
  void write(const std::string& filename) const;
  double aspect_ratio() const {
    return double(xres)/double(yres);
  }
  int getXresolution() {
    return xres;
  }
  int getYresolution() {
    return yres;
  }
 protected:
  Pixel** data;
  int xres, yres;
  Image(const Image&);
  Image& operator=(const Image&);
};

#endif
