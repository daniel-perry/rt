/* Daniel Perry
 * written for cs6620
 * started 3 Mar 05
 */


/*
 * TentFilter : class for filtering
 *    
 */


#ifndef _TENTFILTER_H
#define _TENTFILTER_H

#include "Filter.h"
#include <math.h>

class TentFilter : public Filter{
 public:
  TentFilter():Filter(1,1){ support = 2;}
  TentFilter( double xw , double yw ):Filter( xw, yw ) { support=fabs(xw)+fabs(yw);}
  double getSupport() const;
  double getWeight( double x , double y ) const;
  void getWeights( double * weights , int samples , double * xs , double * ys ) const;
  
 private:
  
};

#endif
