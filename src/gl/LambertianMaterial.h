/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 */

/* LambertianMaterial: material using the lambertian shading model.
 *
 *
 */


#ifndef _LAMBERTIANMATERIAL_H
#define _LAMBERTIANMATERIAL_H

#include "Material.h"
#include "RenderContext.h"
#include "HitRecord.h"
#include "rgb.h"

class LambertianMaterial : public Material
{
 public:
 
  LambertianMaterial( const rgb & m , double kd, double ka ){matte = m; Kd = kd; Ka = ka;}
  void preprocess(){}
  void shade( rgb & result, 
              const RenderContext & context, 
              const ray & r, 
              HitRecord & hit, 
              int depth, 
              double attenuation) const;


  
  double Kd;
  double Ka;
  rgb  matte;

 private:
  LambertianMaterial(){
    matte = rgb(.8,.2,.2);
    Kd = .6;
    Ka = .4;
  }
};

#endif
