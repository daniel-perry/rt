/* Daniel Perry
 * for cs6620 - spring 2005
 * started 7feb05
 */


/* Disk: basic disk class.
 *
 *
 */

#ifndef _DISK_H
#define _DISK_H

#include "Primitive.h"
#include "BBox.h"

class Disk : public Primitive
{
 public:
  Disk( const vector3d & Center , const vector3d & nNormal, double Radius , Material * matl){
    center = Center;
    Normal = nNormal;
    Normal.MakeUnitVector();
    d = dot( Normal , center );
    radius = Radius;
    material = matl;
  }
  Disk( Material * matl, const vector3d & Center , const vector3d & nNormal, double Radius){
    center = Center;
    Normal = nNormal;
    Normal.MakeUnitVector();
    d = dot( Normal , center );
    radius = Radius;
    material = matl;
  }
  
  bool intersect( HitRecord & hit , const RenderContext & context, const ray & r );
  
  // normal returned is guaranteed to be normalized.
  vector3d normal( const vector3d & p ) const{
    return Normal;
  }

  void preprocess(){}

  char *  print(){return "Disk";}

  BBox getBBox();

  vector3d center, Normal;
  double radius,d;
  Material * material;

 private:
  Disk(){}
};

#endif
