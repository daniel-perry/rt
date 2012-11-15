/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 */

/* Material: superclass for all materials
 *
 * direct subclasses: LambertianMaterial, PhongMaterial, DielectricMaterial, Metal Material.
 *
 */


#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "rgb.h"
class RenderContext;
//#include "RenderContext.h"
#include "ray.h"
class HitRecord;
//#include "HitRecord.h"

class Material
{
 public:
  virtual void preprocess()=0;
  virtual void shade( rgb & result, const RenderContext & context, const ray & r, HitRecord & hit, int depth, double attenuation) const = 0;

};

#endif
