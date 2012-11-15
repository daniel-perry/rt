/* Daniel Perry
 * for cs6620 - spring 2005
 * started 12feb05
 */

/* PhongMaterial: material using the phong shading model.(provides for phong highlights).
 *
 *
 */

#ifndef _PHONGMATERIAL_H
#define _PHONGMATERIAL_H

#include "Material.h"
#include "RenderContext.h"
#include "HitRecord.h"
#include "rgb.h"

class PhongMaterial : public Material
{
 public:
  PhongMaterial( const rgb & m, double kd, double ka , const rgb & r , double exp ){
    matte = m;
    Kd = kd;
    Ka = ka;
    reflective = r;
    Exponent = exp;
  }

  
  void preprocess(){}
  
  void shade( rgb & result, const RenderContext & context, const ray & r, HitRecord & hit, int depth, double attenuation) const;

  
  double Kd;
  double Ka;
  double Exponent;
  rgb  matte;
  rgb  reflective;

 private:
  PhongMaterial(){}
};

#endif
