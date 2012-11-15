/* Daniel Perry
 * written for cs6620
 * started 11 Mar 05
 */


/*
 * GaussianFilter : class for filtering
 *    
 */

#include "GaussianFilter.h"
#include <math.h>

GaussianFilter::GaussianFilter():Filter(2,2){ 
  support = 2; 
  alpha = .5;
  expX = exp( -alpha * xWidth * xWidth );
  expY = exp( -alpha * yWidth * yWidth );
}

GaussianFilter::GaussianFilter(double a):Filter(2,2){ 
  support = 2;
  alpha = a;
  expX = exp( -alpha * xWidth * xWidth );
  expY = exp( -alpha * yWidth * yWidth );  
}

GaussianFilter::GaussianFilter( double xw , double yw ,double a):Filter( xw, yw ) { 
  support=fabs(xw);
  alpha = a;
  expX = exp( -alpha * xWidth * xWidth );
  expY = exp( -alpha * yWidth * yWidth );  

}

double GaussianFilter::getSupport() const{
  return support;
}

double GaussianFilter::getWeight( double x , double y ) const{
  return gaussian( x , expX ) * gaussian( y , expY );
}

void GaussianFilter::getWeights( double * weights , int samples , double * xs , double * ys ) const{  
  for(int i=0;i<samples;i++){
    weights[i] = getWeight(xs[i],ys[i]);
  }
}

double GaussianFilter::gaussian( double d , double expv )const{
  return fmax( 0. , exp( -alpha * d * d ) - expv );
}
