/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 */


/* Plane: basic plane class.
 *
 *
 */

#ifndef _PLANE_H
#define _PLANE_H

#include "Primitive.h"
#include "vector3d.h"
#include "util.h"
#include <cmath>
#include <string>
#include <sstream>

class Plane : public Primitive
{
 public:
 
  Plane(const vector3d & nnormal , const vector3d & npoint, Material * matl ){ 
    norm = nnormal/nnormal.length(); 
    point = npoint; 
    material = matl;
    d = dot( norm , point );
    
  }
  // to use same constructor layout as Steve.
  Plane(Material * matl, const vector3d & nnormal , const vector3d & npoint  ){ 
    norm = nnormal/nnormal.length(); 
    point = npoint; 
    material = matl;
    d = dot( norm , point );
    
  }
  
  bool intersect( HitRecord & hit , const RenderContext & context, const ray & r ) const{
    double cosTheta = dot( r.direction() , norm );
    if( fabs(cosTheta) > MYMIN ){ // not parallel to plane, and so has to intersect it(considering an infinate plane). - MYMIN acts as a sort of cut-off point as well.
      double t = (d - dot( r.origin() , norm)) / cosTheta;
      return hit.hit( t, norm, this , material );
    }
    return false; // parallel to plane, treating that case as non-intersecting.
    
   
  }

  std::string toString() {
    std::stringstream s;
    s << "Plane (pt=" << point << "; norm=" << norm  << ")";
    return s.str();
  }

  BBox getBBox() {

    //ray r(point,norm);
    //Point above = r.eval(0.1);
    //Point below = r.eval(-0.1);

    // for now just use a huge bounding box..
    // TODO: restrict the bbox by the delta +/- surface of the plane

    Point min( -MYMAX , -MYMAX , -MYMAX) , 
    max ( MYMAX, MYMAX, MYMAX ); 

    return BBox( min , max );
  } 

  // normal returned is guaranteed to be normalized.
  vector3d  normal( const vector3d & p ) const{ return norm;}

  void preprocess(){}

  
  vector3d norm;
  vector3d point;
  double d;
  Material * material;

 private:
  Plane(){
    point=vector3d(0,0,0); 
    norm=vector3d(1,0,0);
    d = dot( norm , point );
    material = 0;
  }

};

#endif
