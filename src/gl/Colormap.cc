/* for cs6620 - provided by instructor
 *
*/

#include "Colormap.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

Colormap::Colormap(const std::string& filename)
{
  ifstream in(filename.c_str());
  if(!in){
    cerr << "Error opening colormap: " << filename << '\n';
    std::exit(1);
  }
  while(in){
    Data d;
    float r,g,b;
    in >> d.value >> r >> g >> b >> d.opacity;
    d.color = Color(r, g, b);
    if(in)
      input_data.push_back(d);
  }
  if(in.bad() || input_data.size() == 0){
    cerr << "Error reading colormap: " << filename << '\n';
    std::exit(1);
  }
  cerr << "Read " << input_data.size() << " colormap entries from " << filename << '\n';
}

Colormap::~Colormap()
{
}

void Colormap::rescale(double stepsize)
{
  scaled_data.resize(input_data.size());
  for(int i=0;i<static_cast<int>(scaled_data.size());i++){
    Data d = input_data[i];
    //d.opacity = 1-std::exp(-d.opacity  * stepsize);
    scaled_data[i] = d;
  }
}
