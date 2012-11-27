/* Daniel Perry
 * for cs6620 - spring 2005
 * started 14feb05
 */

/* MetalMaterial: material phong shading model.with reflectance to look like metal.
 *
 *
 */

#include "Material.h"

class MetalMaterial : public Material
{
 public:
  MetalMaterial( const rgb & clr , double exp ){
    Exponent = exp;
    Reflective = clr;

  }

  void preprocess(){}
  
  void shade( rgb & result, const RenderContext & context, const ray & r, HitRecord & hit, int depth, double attenuation) const;


  rgb Reflective;
  double Exponent;

 private:
  MetalMaterial(){}

};
