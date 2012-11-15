/* Daniel Perry
 * written for cs6620
 * started 11 Mar 05
 */


/*
 * LanczosSincFilter : class for filtering
 *    
 */

#include "LanczosSincFilter.h"
#include <math.h>
#include "util.h"

#include <iostream>
using namespace std;

double LanczosSincFilter::getSupport() const{return support;}

double LanczosSincFilter::getWeight( double x , double y ) const{
  
  double temp = Sinc1D( x / xWidth ) * Sinc1D( y / yWidth );
  cerr<<"getWeight("<<x<<","<<y<<") = "<<temp<<endl;
  return temp;
}


void LanczosSincFilter::getWeights( double * weights , int samples , double * xs , double * ys ) const{
  for(int i=0;i<samples;i++){
    weights[i] = getWeight(xs[i],ys[i]);
  }
}
  
double LanczosSincFilter::Sinc1D ( double x ) const {
  cerr<<"sinc1d("<<x<<")"<<endl;

  x = fabs( x );
  if ( x < MYMIN ) return 1.;
  if ( x > 1.) return 0.;

  x*= M_PI;

  float sinc = sin( x*tau ) / (x*tau);
  float lanczos = sin( x ) / x;
  return sinc * lanczos;
}
