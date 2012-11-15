/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 */

/* ConstantBackground: background that returns a solid color.
 *
 * 
 *
 */

#ifndef _CONSTANTBACKGROUND_H
#define _CONSTANTBACKGROUND_H

#include "Background.h"
#include "rgb.h"
#include "ray.h"
class RenderContext;

class ConstantBackground : public Background
{
 public:
  ConstantBackground(){color = rgb(.1,.1,.1);}
  ConstantBackground(const rgb & bg){ color = bg;}
  void preprocess(){}
  void getBackgroundColor( rgb & result, const RenderContext & context, const ray & r ){ result = color;}


  rgb color;
};


#endif
