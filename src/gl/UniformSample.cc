/* Daniel Perry
 * written for cs6620
 * started 5 Mar 05
 */


/*
 * UniformSample : class for sampling with uniform distribution.
 *     
 */

#include "UniformSample.h"
//#include "FunctionEval.h"
#include <math.h>
#include <iostream>
using namespace std;

void UniformSample::sample( int x , int y , Color & clr ){
  clr = Color(0,0,0);
  double total_weight = 0,temp_weight;

  //assumes numSamples is a perfect square:
  int sqrt_numSamples = (int)(sqrt((double)numSamples));
  Color temp_clr;
  ray r;

  // convert from pixel space to [-1,1]x[-1,1] space.
  // x_real, y_real should give us the coordinate of the left-bottom corner.
  x_center = 2 * (x-(scene->imageX)/2. + .5 ) / (scene->imageX);
  y_center = 2 * (y-(scene->imageY)/2. + .5 ) / (scene->imageY);
  double x_real = 2 * (x-(scene->imageX)/2. ) / (scene->imageX);
  //double y_real = 2 * (y-(scene->imageY)/2. ) / (scene->imageY);
  //pixel_half_width = x_center - x_real;
  pixel_half_width = (x_center - x_real) * 2;// ((scene->filter->xWidth)/.5);
  //cerr<<"pix_h1:"<<(x_center - x_real ) <<" pix_h2:"<<pixel_half_width<<endl;
  //cerr<<"numSamples: "<<numSamples<<" sqrt(): "<<sqrt_numSamples<<endl;
  //cerr<<"pixel: ("<<x<<", "<<y<<")"<<endl;

  for( int i=0; i<sqrt_numSamples ; i++){
    for( int j=0; j<sqrt_numSamples ; j++){
      // get offset to add to corner.
      double x_offset = ((double)i + .5) / (double)sqrt_numSamples;
      double y_offset = ((double)j + .5) / (double)sqrt_numSamples;
      // convert offset plus left bottom corner to [-1,1]x[-1,1] space.
      x_offset = 2 * (x-(scene->imageX)/2. + x_offset ) / (scene->imageX);
      y_offset = 2 * (y-(scene->imageY)/2. + y_offset ) / (scene->imageY);


      //cerr<<"sample: ("<<i<<", "<<j<<") corner_coords: ("<<x_real<<", "<<y_real<<") offset_coords: ("<<x_offset<<", "<<y_offset<<")"<<endl;

#ifdef USE_FUNCTION_  // use function instead of tracing.
      FunctionEval( x_offset , y_offset , temp_clr );
#else
      scene->camera->makeRay( r , scene->rc , x_offset , y_offset );
      scene->render( temp_clr , r );
#endif

      temp_weight = scene->filter->getWeight( (x_offset-x_center)/pixel_half_width , (y_offset-y_center)/pixel_half_width );
      //cerr<<"weight:"<<temp_weight<<" x :"<<(x_offset-x_center)/pixel_half_width <<" y:"<<(y_offset-y_center)/pixel_half_width <<endl<<"temp clr:"<<temp_clr<<endl;
      clr += (temp_clr* temp_weight );
      total_weight += temp_weight;
      

    }
  }

  // if the filter supports incorporating samples from other pixels, need to get them:
  if( scene->filter->getSupport() > 1 ){
    // these points should be the limit of the filter.
    double x_limit1 = 2 * (x-(scene->imageX)/2. + .5 + scene->filter->xWidth ) / (scene->imageX);
    double y_limit1 = 2 * (y-(scene->imageY)/2. + .5 + scene->filter->yWidth ) / (scene->imageY);
    double x_limit2 = 2 * (x-(scene->imageX)/2. + .5 - scene->filter->xWidth ) / (scene->imageX);
    double y_limit2 = 2 * (y-(scene->imageY)/2. + .5 - scene->filter->yWidth ) / (scene->imageY);
    //cerr<<"pixel:("<<x<<","<<y<<") limits: ("<<x_limit1<<", "<<y_limit1<<"),("<<x_limit2<<","<<y_limit2<<")"<<endl;

    total_weight += sample( x + 1 , y , x_limit1 , y_limit1, x_limit2 , y_limit2 , temp_clr );
    clr += temp_clr; 
    total_weight += sample( x + 1 , y + 1 , x_limit1 , y_limit1 , x_limit2 , y_limit2 , temp_clr );
    clr += temp_clr;
    total_weight += sample( x  , y + 1 , x_limit1 , y_limit1 , x_limit2 , y_limit2 , temp_clr );
    clr += temp_clr;
    total_weight += sample( x - 1 , y + 1 , x_limit1 , y_limit1 , x_limit2 , y_limit2 , temp_clr );
    clr += temp_clr;
    total_weight += sample( x - 1 , y  , x_limit1 , y_limit1 , x_limit2 , y_limit2 , temp_clr );
    clr += temp_clr;
    total_weight += sample( x - 1 , y -1 , x_limit1 , y_limit1 , x_limit2 , y_limit2 , temp_clr );
    clr += temp_clr;
    total_weight += sample( x , y -1 , x_limit1 , y_limit1 , x_limit2 , y_limit2 , temp_clr );
    clr += temp_clr;
    total_weight += sample( x + 1  , y -1 , x_limit1 , y_limit1 , x_limit2 , y_limit2 , temp_clr );
    clr += temp_clr;
  }

  
  //cerr<<"total weight:"<<total_weight<<" total clr:"<<clr<<endl;
  clr /= total_weight;
 
}


// this is more of a utility funciton for the above sample() function.
double UniformSample::sample( int x , int y , double x_limit1 , double y_limit1 , double x_limit2, double y_limit2 , Color & clr ) const{
  clr = Color(0,0,0);
  double total_weight = 0, temp_weight;

  int sqrt_numSamples = (int)(sqrt((double)numSamples));
  Color temp_clr;
  ray r;

  // convert from pixel space to [-1,1]x[-1,1] space.
  // this should give us the coordinate of the left-bottom corner.
  //double x_real = 2 * (x-(scene->imageX)/2. ) / (scene->imageX);
  //double y_real = 2 * (y-(scene->imageY)/2. ) / (scene->imageY);

  //cerr<<"numSamples: "<<numSamples<<" sqrt(): "<<sqrt_numSamples<<endl;
  //cerr<<"pixel2: ("<<x<<", "<<y<<"): limit:"<<x_limit<<","<<y_limit<<endl;

  for( int i=0; i<sqrt_numSamples ; i++){
    for( int j=0; j<sqrt_numSamples ; j++){
      // get offset to add to corner.
      double x_offset = ((double)i + .5) / (double)sqrt_numSamples;
      double y_offset = ((double)j + .5) / (double)sqrt_numSamples;
      // convert offset plus left bottom corner to [-1,1]x[-1,1] space.
      x_offset = 2 * (x-(scene->imageX)/2. + x_offset ) / (scene->imageX);
      y_offset = 2 * (y-(scene->imageY)/2. + y_offset ) / (scene->imageY);

      // check to make sure the sample will be used:(this essentially makes anything outside the support zero.)
      if( x_offset <= x_limit1 && y_offset <= y_limit1 && x_offset >= x_limit2 && y_offset >= y_limit2){//cerr<<"used."<<endl;}
	//cerr<<"sample2: ("<<i<<", "<<j<<") realcoords: ("<<x_offset<<", "<<y_offset<<")"<<endl;
#ifdef USE_FUNCTION_  // use function instead of tracing.
	FunctionEval( x_offset , y_offset , temp_clr );
#else
	scene->camera->makeRay( r , scene->rc , x_offset , y_offset );
	scene->render( temp_clr , r );
#endif

	temp_weight = scene->filter->getWeight( (x_offset-x_center)/pixel_half_width , (y_offset-y_center)/pixel_half_width );

      //cerr<<"weight:"<<temp_weight<<" x :"<<(x_offset-x_center)/pixel_half_width <<" y:"<<(y_offset-y_center)/pixel_half_width <<endl;

        clr += (temp_clr * temp_weight);
	total_weight += temp_weight;
      }
    }
  }

  return total_weight;

}
