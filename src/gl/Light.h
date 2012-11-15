/* Daniel Perry
 * for cs6620 - spring 2005
 * started 27jan05
 */

/* Light: superclass of different types of lights
 *
 * direct subclasses: PointLight, DirectionalLight
 *
 */

#ifndef _LIGHT_H
#define _LIGHT_H

using namespace std;

#include "rgb.h"
#include "vector3d.h"
#include "hpoint.h"
//#include "RenderContext.h"
class RenderContext;

class Light
{
 public:
  virtual void preprocess()=0;
  virtual double getLight( rgb & light_color , vector3d & light_direction , const RenderContext & context , const vector3d & pos) const = 0;

 private:
};


#endif
