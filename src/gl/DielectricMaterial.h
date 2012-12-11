/* Daniel Perry
 * for cs6620 - spring 2005
 * started 14feb05
 */

/* DielectMaterial: transparent material (glass, clear plastic, etc.)
 *
 *
 */

#ifndef _DIELECTRICMATERIAL_H
#define _DIELECTRICMATERIAL_H

#include "Material.h"

class DielectricMaterial : public Material
{
 public:
  DielectricMaterial(double eta, double exp):Refraction(eta),Exponent(exp){ preprocess();}

  void preprocess(){
    R = (Refraction-1)/(Refraction+1);
    R*=R;
  }

  void shade( rgb & result, 
              const RenderContext & context, 
              const ray & r, 
              HitRecord & hit, 
              int depth, 
              double attenuation) const;

  double Refraction; // index of refraction
  double Exponent; // specular exponent
  double R;

 private:
  DielectricMaterial(){}

};


#endif
