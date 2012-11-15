#ifndef _RAY_H
#define _RAY_H

#include "vector3d.h"
#include <math.h>

class ray
{
  
public:  
  ray(const vector3d & ov, const vector3d & dv)
    :o(ov), d(dv)
  { }
  
  vector3d origin() const
  {
    return o;
  }

  vector3d direction() const
  {
    return d;
  }

  void setorigin(const vector3d & ov)
  {
    o = ov;
  }

  void setdirection( const vector3d & dv )
  {
    d = dv;
  }

  vector3d eval ( double t ) const
  {
    return origin() + t*direction();
  }

 private:
  vector3d o, d;
};

ostream & operator << ( ostream & os , ray r );

bool ray_sphere_intersect(const ray& r, const vector3d& c, double radius, double tmin, double tmax, double& t);
bool ray_triangle_intersect(const ray& r,
			    const vector3d& p0, const vector3d& p1, const vector3d& p2,
			    double tmin, double tmax, double& beta, double& gamma, double& t);
bool ray_box_intersect(const ray& r, const vector3d& p0, const vector3d& p1, double ta_min, double ta_max, vector3d& n, double& t);
#endif










