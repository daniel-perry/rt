/* Daniel Perry
 * written for cs6620
 * started 2 Mar 05
 */


/*
 * BoxFilter : class for filtering
 *    
 */

#include "BoxFilter.h"

double BoxFilter::getSupport() const{
  return support;
}

double BoxFilter::getWeight( double x , double y ) const{
  return 1.;
}

void BoxFilter::getWeights( double * weights , int samples , double * xs , double * ys ) const{
  for( int i=0; i< samples ; i++ ){
    weights[i] = getWeight( xs[i] , ys[i] );
  }
}
