/* Daniel Perry
 * for cs6620 - spring 2005
 * started 27jan05
 */

/* DirectionalLight: light type with just direction and color.
 *
 *
 */


#ifndef _DIRECTIONALLIGHT_H
#define _DIRECTIONALLIGHT_H

#include "Light.h"
#include "RenderContext.h"
#include "vector3d.h"
#include "hpoint.h"
#include "rgb.h"

class DirectionalLight : public Light
{
 public:
  DirectionalLight(const rgb & clr, const vector3d & dir){ lcolor = clr; ldirection = dir/dir.length();}
  DirectionalLight(){ lcolor = rgb(.8,.8,.8); ldirection = vector3d(0,-1,0);} // default a semi-bright white pointing straight down.
  void preprocess(){}
  double getLight( rgb & light_color , vector3d & light_direction , const RenderContext & context , const vector3d & pos) const { light_color = lcolor; light_direction = ldirection; return 1; }
  


  //private:
  rgb lcolor;
  vector3d ldirection;
};


#endif
