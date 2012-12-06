/* Daniel Perry
 * written for cs6620
 * started 2 Mar 05
 */


/*
 * Sample : super class for sampling classes
 *     subclasses: JitteredSample, etc.
 */

#ifndef _SAMPLE_H
#define _SAMPLE_H

#include "util.h" // for USE_FUNCTION_ definition.
#include "FunctionEval.h"

#include "vector3d.h"
#include "rgb.h"
#include "Scene.h"

class Sample{
 public:

  Sample( int ns , Scene * sc){
    numSamples = ns;  
    //sampleColors = new Color[numSamples];
    scene = sc;
  }

  // samples entire pixel (x,y) and returns filtered color of entire pixel
  virtual void sample( int x , int y , Color & clr ) const = 0;

  // returns sample (i,j) of pixel (x,y)
  //virtual void sample( int x , int y , int i , int j , Color & clr ) const = 0;

  // similar to sample() but tests to make sure is needed before rendering (tracing)
  // returns the sum of weights (to use in final sampling) - clr is sum of all sampled colors.
  //virtual double sample( int x , int y , double x_limit , double y_limit , Color & clr ) const = 0;
  virtual double sample( int x , int y , double x_limit1 , double y_limit1 , double x_limit2, double y_limit2 , Color & clr ) const = 0;


  //virtual void sample( int x, int y) = 0;// fills sampleColors[] with the samples.

  int numSamples;
  //double x_center, y_center, pixel_half_width;
  //Color *sampleColors;
  Scene * scene;

 private:
};

#endif
