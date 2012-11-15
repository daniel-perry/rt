#ifndef _AXISBOX_H
#define _AXISBOX_H

#include "surface.h"
#include "rgb.h"
#include "triangle.h"
#include "vector3d.h"

class axisbox : public surface

{
 public:
  axisbox();
  axisbox( const vector3d & np0, const vector3d & np1, const rgb & kd, const rgb & ks, double kh, double e );
  
  // doing this in surface.h
  //~axisbox()
  //{
  //  delete myTexture;
  //}

  vector3d p0() const
  {
    return myP0;
  }
  vector3d p1() const
  {
    return myP1;
  }

  void setp0( const vector3d & p0 )
  {
    myP0 = vector3d(p0);
  } 
  void setp1( const vector3d & p1 )
  {
    myP1 = vector3d(p1);
  }

  bool hit (const ray & r, double tmin, double tmax, double & t, surfacevals & sv ) const; 

  vector3d centroid() const;
  dynarray<vector3d> elements() const;
  dynarray<triangle> triangulate() const; 

  //private:
  vector3d myP0, myP1;
};

#endif



