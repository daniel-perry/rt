/* Daniel Perry
 * written for cs6620
 * started 2 Mar 05
 */


/*
 * JitteredSample : class for sampling pixels.
 *     
 */

#ifndef _JITTEREDSAMPLE_H
#define _JITTEREDSAMPLE_H

#include "Sample.h"
#include "vector3d.h"
#include "Scene.h"
/*#include <stdlib.h>
#include <time.h>
    time_t temp;
    unsigned short temp2 = (unsigned short)time(&temp);
    seed48(&temp2);
*/

class JitteredSample : public Sample{

 public:
  // assuming ns is a perfect square
  JitteredSample( int ns , Scene * sc):Sample(ns,sc) { }
  void sample( int x , int y , Color & clr ) const;
  double sample( int x , int y , double x_limit1 , double y_limit1 , double x_limit2, double y_limit2 , Color & clr ) const;
 private:
  //JitteredSample(){}

};

#endif
