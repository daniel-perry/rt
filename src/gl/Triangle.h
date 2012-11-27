/* Daniel Perry
 * for cs6620 - spring 2005
 * started 7feb05
 */


/* Triangle: basic triangle class.
 *
 *
 */


#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "Primitive.h"

class Triangle : public Primitive
{
 public:

  Triangle( const vector3d & np0, const vector3d & np1, const vector3d & np2 , Material * matl ){
    p0 = np0;
    p1 = np1;
    p2 = np2;
    material = matl;
    Normal = cross( p1-p0 , p2-p0 );
    Normal.MakeUnitVector();
  }

  Triangle( Material * matl , const vector3d & np0, const vector3d & np1, const vector3d & np2 ){
    
    p0 = np0;
    p1 = np1;
    p2 = np2;
    material = matl;
    Normal = cross( p1-p0 , p2-p0 );
    Normal.MakeUnitVector();

  }

  void preprocess(){}

  std::string toString(){ return "Triangle";}

  vector3d normal( const vector3d & p ) const{
    return Normal;
  }

  
  BBox getBBox(); 

  bool intersect( HitRecord & hit , const RenderContext & context, const ray & r );

  vector3d p0,p1,p2,Normal;
  Material * material;

 private:
  Triangle(){}

};

#endif
