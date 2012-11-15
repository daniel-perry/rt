
#include "Image.h"
#include <fstream>

Image::Image(int xres, int yres)
  : xres(xres), yres(yres)
{
  data = new Pixel*[yres];
  Pixel* p = new Pixel[xres*yres];
  for(int i=0;i<yres;i++)
    data[i] = p + i*xres;
}

Image::~Image()
{
  delete[] data[0];
  delete[] data;
}

void Image::write(const std::string& filename) const
{
  std::ofstream out(filename.c_str());
  out << "P6 " << xres << " " << yres << " 255\n";
  for(int i=yres-1;i>=0;i--)
    out.write(reinterpret_cast<char*>(data[i]), sizeof(Pixel)*xres);
}
