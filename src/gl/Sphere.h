/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 */


/* Sphere: basic sphere class.
 *
 *
 */


#ifndef _SPHERE_H
#define _SPHERE_H

using namespace std;

#include "Primitive.h"
#include "LambertianMaterial.h"

class Sphere : public Primitive
{
 public:
  Sphere( const vector3d & ctr , double rad , Material * matl ){ center = ctr; radius = rad; material = matl;}
  // to agree with Steve's constructor calls.
  Sphere( Material * matl, const vector3d & ctr , double rad ){ center = ctr; radius = rad; material = matl;
  }

  bool intersect( HitRecord & hit , const RenderContext & context, const ray & r ) const;
  
  // normal returned is guaranteed to be normalized.
  vector3d normal( const vector3d & p ) const {
    return vector3d((p-center)/radius);
  }

  void preprocess(){}

  std::string toString(){return "Sphere";}

  BBox getBBox();

  vector3d center;
  double radius;
  Material * material;

 private:
  Sphere(){}
  //Sphere(){ center = hpoint(0,0,1,0); radius = .4; material = new LambertianMaterial(); }

};

#endif
