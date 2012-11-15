/* Daniel Perry
 * written for cs6620
 * started 2 Mar 05
 */


/*
 * BoxFilter : class for filtering
 *    
 */

#ifndef _BOXFILTER_H
#define _BOXFILTER_H

#include "Filter.h"
#include <math.h>

class BoxFilter : public Filter{
  
 public:
  BoxFilter():Filter(.5,.5) { support = 1;}
  BoxFilter( double xw , double yw ):Filter(xw,yw) { support = fabs(xWidth)+fabs(yWidth);}

  double getSupport() const;
  double getWeight( double x , double y ) const;
  void getWeights( double * weights , int samples , double * xs , double * ys ) const;

  //private:
  
};

#endif
