#ifndef _SPHERE_H
#define _SPHERE_H

#include "surface.h"
#include "rgb.h"
#include "vector3d.h"

class sphere : public surface
{
 public:
  sphere();
  sphere( const vector3d & c, double r, const rgb & kd, const rgb & ks, double kh, double e );
  
  double radius() const
  {
    return myRadius;
  }
  vector3d center() const
  {
    return myCenter;
  }
 

  void setradius( double r )
  {
    myRadius = r;
  }
  void setcenter( const vector3d & c )
  {
    myCenter = vector3d(c);
  }
  

  vector3d normal( const vector3d & p ) const
  {
    vector3d temp = p-center();
    return ( temp / temp.length() );
  }

  bool hit (const ray & r, double tmin, double tmax, double & t, surfacevals & sv ) const; 
  
  vector3d centroid() const;
  dynarray<vector3d> elements() const;
  
  //private:
  vector3d myCenter;
  double myRadius;
  
};


#endif

