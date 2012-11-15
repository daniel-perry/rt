/* Daniel Perry
 * written for cs6620
 * started 11 Mar 05
 */


/*
 * GaussianFilter : class for filtering
 *    
 */

/*
 note that there is a slight discrepancy between filter use: Tent and Box constructor ask for half the support width (I misunderstood the definition when I made them)
 while Gaussian filter takes the full size of support in both the x and y directions.
 */

#ifndef _GAUSSIANFILTER_H
#define _GAUSSIANFILTER_H

#include "Filter.h"

class GaussianFilter : public Filter{
 public:
  GaussianFilter();
  GaussianFilter(double a);
  GaussianFilter( double xw , double yw ,double a);
  double getSupport() const;
  double getWeight( double x , double y ) const;
  void getWeights( double * weights , int samples , double * xs , double * ys ) const;
  
  double gaussian( double d , double expv )const;

  
  double alpha;
  double expX, expY;

 private:
  
};

#endif
