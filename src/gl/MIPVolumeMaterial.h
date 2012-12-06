/* MIPVolumeMaterial: volume rendering using the Phong lighting model.
 */


#ifndef _MIPVolumeMaterial_H
#define _MIPVolumeMaterial_H

#include "Material.h"
#include "Colormap.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class MIPVolumeMaterial : public Material
{
 public:
  MIPVolumeMaterial(const std::string& headername,
                      const std::string& cmapname,
                      const Point& lower, 
                      const Point& upper,
                      double grid_stepsize, 
                      float maxopacity,
                      bool nearest_neighbor = true
                      );

  void preprocess();
  void shade( rgb & result, 
              const RenderContext & context, 
              const ray & r, 
              HitRecord & hit, 
              int depth, 
              double attenuation) const;
  
  Colormap cmap;
  Point lower, upper, bounds[2];
  double grid_stepsize, world_stepsize;
  float maxopacity;
  bool nearest;
  
  Point diag, cellsize;
  short *** data; //3d array.
  int size1,size2,size3; //size of each array dimension.

};

#endif
