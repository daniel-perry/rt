#ifndef _BOUNDINGBOX_H
#define _BOUNDINGBOX_H

#include "vector3d.h"
#include "surface.h"

class boundingbox
{
 public:
  boundingbox( const vector3d & p1, const vector3d & p2)
    :p1(p1),p2(p2)
  {}

  bool hit( const ray & r, double tmin, double tmax ) const;
  
  vector3d point1() const { return p1; }
  vector3d point2() const { return p2; }

  vector3d p1, p2;

 private:
  boundingbox();  // no default constructing.
  
};

ostream & operator << ( ostream & os , const boundingbox & box );
bool ray_bbox_intersect(const ray& r, const vector3d& p0, const vector3d& p1, double ta_min, double ta_max);
#endif










