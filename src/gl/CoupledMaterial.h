/* Daniel Perry
 * for cs6620 - spring 2005
 * started 15feb05
 */

/* CoupledMaterial: dual surfaced material (gloss layer on top of another).
 *
 *
 */


#ifndef _COUPLEDMATERIAL_H
#define _COUPLEDMATERIAL_H

#include "Material.h"
#include "RenderContext.h"
#include "HitRecord.h"
#include "rgb.h"

class CoupledMaterial : public Material
{
 public:
  CoupledMaterial(const rgb & surface,double kd, double ka,const rgb & phong, double exp){
    Surface = surface;
    Phong = surface;
    Exponent = exp;
    Ka = ka;
    Kd = kd;
  }

   void preprocess(){}
  
  void shade( rgb & result, const RenderContext & context, const ray & r, const HitRecord & hit, int depth, double attenuation) const;


  rgb Surface;
  rgb Phong;
  double Exponent;
  double Ka,Kd;

 private:
  CoupledMaterial(){}
};


#endif
