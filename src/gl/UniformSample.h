/* Daniel Perry
 * written for cs6620
 * started 5 Mar 05
 */


/*
 * UniformSample : class for sampling with uniform distribution.
 *     
 */

#ifndef _UNIFORMSAMPLE_H
#define _UNIFORMSAMPLE_H

#include "Sample.h"

class UniformSample : public Sample{
 public:
  UniformSample( int ns , Scene * sc ):Sample(ns,sc){}
  void sample( int x , int y , Color & clr );
  double sample( int x , int y , double x_limit1 , double y_limit1 , double x_limit2, double y_limit2 , Color & clr ) const;
};




#endif
