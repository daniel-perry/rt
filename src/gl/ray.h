/* Daniel Perry
 * originally written in fall 2001 for cs3505 - changed and updated Feb05
 *
 */

#ifndef _RAY_H
#define _RAY_H

#include "vector3d.h"
#include <cmath>

using std::ostream;
using std::istream;

class ray
{
  
public:  
  ray(){}
  ray(const vector3d & ov, const vector3d & dv)
    :o(ov), d(dv)
  { 
    inv_d = vector3d( 1/d.x() , 1/d.y() , 1/d.z() );
    sign[0] = (inv_d.x()<0);//?1:0;
    sign[1] = (inv_d.y()<0);//?1:0;
    sign[2] = (inv_d.z()<0);//?1:0;
  }
  
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
    inv_d = vector3d( 1/d.x() , 1/d.y() , 1/d.z() );
    sign[0] = (inv_d.x()<0);//?1:0;
    sign[1] = (inv_d.y()<0);//?1:0;
    sign[2] = (inv_d.z()<0);//?1:0;
  }

  vector3d eval ( double t ) const
  {
    return origin() + t*direction();
  }

  //private:
  vector3d o, d, inv_d;
  int sign[3];
};

ostream & operator << ( ostream & os , ray r );


bool ray_sphere_intersect(const ray& r, const vector3d& c, double radius, double tmin, double tmax, double& t);


bool ray_triangle_intersect(const ray& r,
			    const vector3d& p0, const vector3d& p1, const vector3d& p2,
			    double tmin, double tmax, double& beta, double& gamma, double& t);
bool ray_box_intersect(const ray& r, const vector3d& p0, const vector3d& p1, double ta_min, double ta_max, vector3d& n, double& t);

#endif










