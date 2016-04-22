/* IsosurfaceVolumeMaterial: isosurface rendering
 */


#ifndef _IsosurfaceVolumeMaterial_H
#define _IsosurfaceVolumeMaterial_H

#include "Material.h"
#include "Colormap.h"

#include <itkImage.h>
#include <itkGradientRecursiveGaussianImageFilter.h>
#include <itkHessianRecursiveGaussianImageFilter.h>

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class IsosurfaceVolumeMaterial : public Material
{
 public:
  IsosurfaceVolumeMaterial(const std::string& data_fn,
											float isoval,
  										const rgb & phong_matte, 
											double phong_kd, 
											double phong_ka , 
											const rgb & phong_r , 
											double phong_exp,
                      const Point& lower, 
                      const Point& upper,
                      bool nearest_neighbor = false
                      );

  void preprocess();
  void shade( rgb & result, 
              const RenderContext & context, 
              const ray & r, 
              HitRecord & hit, 
              int depth, 
              double attenuation) const;

	float isovalue;
  rgb  matte;
  double Kd;
  double Ka;
  rgb  reflective;
  double Exponent;

  Point lower, upper, bounds[2];
  double grid_stepsize, world_stepsize;
  bool nearest;
  
  Point diag, cellsize;
  //short *** data; //3d array.
  typedef float PixelType;
  typedef itk::Image<PixelType,3> ImageType;
  typedef itk::GradientRecursiveGaussianImageFilter<ImageType> GradientFilter;
  typedef GradientFilter::OutputImageType GradientImageType;
  typedef GradientImageType::PixelType GradientType;
  ImageType::Pointer data; 
  GradientImageType::Pointer gradient; // first derivative

  bool normalFlipped;

  int size1,size2,size3; //size of each array dimension.

};

#endif
