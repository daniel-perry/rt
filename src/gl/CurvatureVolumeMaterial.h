/* CurvatureVolumeMaterial: volume rendering using curvature..
 */


#ifndef _CurvatureVolumeMaterial_H
#define _CurvatureVolumeMaterial_H

#include "Material.h"
#include "Colormap.h"

#include <itkImage.h>
#include <itkGradientRecursiveGaussianImageFilter.h>
#include <itkHessianRecursiveGaussianImageFilter.h>

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class CurvatureVolumeMaterial : public Material
{
 public:
  //CurvatureVolumeMaterial(const std::string& headername,
  CurvatureVolumeMaterial(const std::string& data_fn,
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
  typedef float PixelType;
  typedef itk::Image<PixelType,3> ImageType;
  typedef itk::GradientRecursiveGaussianImageFilter<ImageType> GradientFilter;
  typedef itk::HessianRecursiveGaussianImageFilter<ImageType> HessianFilter; 
  typedef GradientFilter::OutputImageType GradientImageType;
  typedef GradientImageType::PixelType GradientType;
  typedef HessianFilter::OutputImageType HessianImageType;
  typedef HessianImageType::PixelType HessianType;
  typedef HessianType::MatrixType MatrixType;
  ImageType::Pointer data; 
  GradientImageType::Pointer gradient; // first derivative
  HessianImageType::Pointer hessian; // second derivative

  int size1,size2,size3; //size of each array dimension.

};

#endif
