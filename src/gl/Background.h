/* Daniel Perry
 * for cs6620 - spring 2005
 * started 27jan05
 */

/* Background: superclass for different types of backgrounds
 *
 * direct subclasses: ConstantBackground, GradientBackground, StarfieldBackground, EnvironmentMapBackground.
 *
 */


#ifndef _BACKGROUND_H
#define _BACKGROUND_H

using namespace std;

class RenderContext;
//#include "RenderContext.h"
#include "ray.h"
#include "rgb.h"

class Background{
 public:
  virtual void preprocess()=0;
  virtual void getBackgroundColor( rgb & result, const RenderContext & context, const ray & r ) = 0;
};


#endif
