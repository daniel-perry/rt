/* Daniel Perry
 * for cs6620 - spring 2005
 * started 27jan05
 */

/* PointLight: light type with just a point and color.
 *
 * 
 */


#ifndef _POINTLIGHT_H
#define _POINTLIGHT_H

#include "Light.h"

class PointLight : public Light
{
 public: 
  PointLight(){ lcolor = rgb(.8,.8,.8); location=vector3d(0,0,0);} // default semi-bright white light at origin.
  PointLight(const rgb & clr, const vector3d & loc){ lcolor = clr; location = loc;}
  PointLight( const vector3d & loc, const rgb & clr){ lcolor = clr; location = loc;}
 
  void preprocess(){}
  double getLight( rgb & light_color , vector3d & light_direction , const RenderContext & context , const vector3d & pos) const { 
    double temp;
    light_color = lcolor; 
    light_direction = (location-pos);
    temp = light_direction.length();
    light_direction.MakeUnitVector(); 
    return temp; 
  }

 

  //private:
  rgb lcolor;
  vector3d location;
};


#endif
