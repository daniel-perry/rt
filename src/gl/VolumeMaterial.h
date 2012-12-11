/* VolumeMaterial: plain old volume rendering.
 */


#ifndef _VolumeMaterial_H
#define _VolumeMaterial_H

#include "Material.h"
#include "Colormap.h"

#include <itkImage.h>

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class VolumeMaterial : public Material
{
 public:
  //VolumeMaterial(const std::string& headername,
  VolumeMaterial(const std::string& data_fn,
                      const std::string& cmap_fn,
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
  //short *** data; //3d array.
  typedef itk::Image<short,3> ImageType;
  ImageType::Pointer data; 

  int size1,size2,size3; //size of each array dimension.

};

#endif
