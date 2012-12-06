/* Daniel Perry
 * for cs6620 - spring 2005
 * started 7feb05
 */


/* AxisBox: basic Axis-aligned box class.
 *
 *
 */


#ifndef _AXISBOX_H
#define _AXISBOX_H

#include "Primitive.h"
#include "util.h"
#include "BBox.h"

class AxisBox : public Primitive
{
 public:
  AxisBox( const vector3d & min , const vector3d & max , Material * matl){
    bounds[0] = min;
    bounds[1] = max;
    material = matl;
  }
  // to aggree with Steves contructor calls.
  AxisBox( Material * matl, const vector3d & min , const vector3d & max){
    bounds[0] = min;
    bounds[1] = max;
    material = matl;
  }
  

  bool intersect( HitRecord & hit , const RenderContext & context, const ray & r ) const ;
  
  // normal returned is guaranteed to be normalized.
  // NOTE that for the AxisBox class, the normal function will only be valid after a hit
  // has been calculated succesfully (because that's where we grab the normal from.
  vector3d normal( const vector3d & p ) const{
    return Normal;
  }

  BBox getBBox(){
    return BBox( bounds[0] , bounds[1] );
  }

  void preprocess(){}

  std::string toString(){return "AxisBox";}

  vector3d bounds[2];
  vector3d Normal; //reset at every hit.
  Material * material;

 private:
  AxisBox(){}
};

// to agree with Steve's calls.
typedef AxisBox Box;

#endif
