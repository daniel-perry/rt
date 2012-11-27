/* Daniel Perry
 * for cs6620 - spring 2005
 * started 7feb05
 */


/* Ring: basic Ring class.
 *
 *
 */

#ifndef _RING_H
#define _RING_H

#include "Primitive.h"

class Ring : public Primitive
{
 public:
  Ring( const vector3d & Center , const vector3d & nNormal, double nsmallRadius, double nlargeRadius, Material * matl){
    center = Center;
    Normal = nNormal;
    Normal.MakeUnitVector();
    d = dot( Normal , center );
    smallRadius = nsmallRadius;
    largeRadius = nlargeRadius;
    material = matl;
  }

  Ring( Material * matl, const vector3d & Center , const vector3d & nNormal, double nsmallRadius, double nlargeRadius){
    center = Center;
    Normal = nNormal;
    Normal.MakeUnitVector();
    d = dot( Normal , center );
    smallRadius = nsmallRadius;
    largeRadius = nlargeRadius;
    material = matl;
  }




  bool intersect( HitRecord & hit , const RenderContext & context, const ray & r );
  
  // normal returned is guaranteed to be normalized.
   vector3d normal( const vector3d & p ) const{
     return Normal;
   }

   void preprocess(){}   
   std::string toString(){return "Ring";}

   BBox getBBox();

   vector3d center, Normal;
   double smallRadius, largeRadius, d;
   Material * material;

 private:
   Ring(){}
};

#endif
