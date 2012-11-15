/* Daniel Perry
 * written for cs6620
 * started 11 Mar 05
 */


/*
 * LanczosSincFilter : class for filtering
 *    
 */
//////////// NOT FULLY DEBUGGED YET ////////////

#ifndef _LANCZOSSINCFILTER_H
#define _LANCZOSSINCFILTER_H

#include "Filter.h"
#include <math.h>

class LanczosSincFilter : public Filter{
 public:
  LanczosSincFilter():Filter(1.5,1.5){ support = 3; tau=3;}
  LanczosSincFilter(double t):Filter(1.5,1.5){ support = 3; tau=t;}
  LanczosSincFilter( double xw , double yw ,double t):Filter( xw, yw ) { support=fabs(xw)+fabs(yw);tau=t;}
  double getSupport() const;
  double getWeight( double x , double y ) const;
  void getWeights( double * weights , int samples , double * xs , double * ys ) const;
  
  double Sinc1D ( double x ) const;

  double tau;
 private:
  
};

#endif
