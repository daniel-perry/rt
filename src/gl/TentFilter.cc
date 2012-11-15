/* Daniel Perry
 * written for cs6620
 * started 3 Mar 05
 */


/*
 * TentFilter : class for filtering
 *    
 */
#include "TentFilter.h"

#include <math.h>
#include <iostream>
using namespace std;


double TentFilter::getSupport() const{
  return support;
}

double TentFilter::getWeight( double x , double y ) const{
  double temp = fmax( 0.f , xWidth - fabs( x ) ) * fmax( 0.f , yWidth - fabs( y ) );  

  //cerr<<"getWeight("<<x<<","<<y<<") = "<<temp<<endl;

  return temp;
  
}

void TentFilter::getWeights( double * weights , int samples , double * xs , double * ys ) const{
  for(int i=0;i<samples;i++){
    weights[i] = getWeight(xs[i],ys[i]);
  }

}
