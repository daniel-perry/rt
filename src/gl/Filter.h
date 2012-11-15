/* Daniel Perry
 * written for cs6620
 * started 2 Mar 05
 */


/*
 * Filter : super class for filtering classes
 *     subclasses: BoxFilter, TentFilter, etc.
 */

/*
 note that there is a slight discrepancy between filter use: Tent and Box constructor ask for half the support width (I misunderstood the definition when I made them)
 while Gaussian filter takes the full size of support in both the x and y directions.
 */


#ifndef _FILTER_H
#define _FILTER_H

class Filter{
 public:
  Filter( double xw , double yw )
    :xWidth(xw),yWidth(yw)
    {}

  virtual double getSupport() const = 0;
  virtual double getWeight( double x , double y ) const = 0;
  virtual void getWeights( double * weights , int samples , double * xs , double * ys ) const = 0;
  
  
  double xWidth,yWidth,support;

  //private:
  Filter(){}
};


#endif 
