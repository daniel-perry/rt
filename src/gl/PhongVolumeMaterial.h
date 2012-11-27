/* Daniel Perry
 * for cs6620 - spring 2005
 * started 28mar05
 */

/* PhongVolumeMaterial: volume rendering using the Phong lighting model.
 */


#ifndef _PHONGVOLUMEMATERIAL_H
#define _PHONGVOLUMEMATERIAL_H

#include "Material.h"
#include "Colormap.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class PhongVolumeMaterial : public Material
{
 public:
  PhongVolumeMaterial(const std::string& headername,
                      const std::string& cmapname,
                      const Point& lower, 
                      const Point& upper,
                      double grid_stepsize, 
                      float maxopacity,
                      float Kd, float Ka,
                      const Color& phong_color,
					            int phong_exponent);

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
  float Kd, Ka;
  Color phong_color;
  int phong_exponent;
  
  Point diag, cellsize;
  short *** data; //3d array.
  int size1,size2,size3; //size of each array dimension.

};

#endif
