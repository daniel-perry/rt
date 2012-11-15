#ifndef _HBV_NODE_H
#define _HBV_NODE_H

#include "surface.h"
#include "boundingbox.h"


class hbv_node : public surface
{
 public:
  hbv_node( const boundingbox & b, surface * l, surface * r )
    :box(b), left(l), right(r)
  {}
  
  bool hit (const ray & r, double tmin, double tmax, double & t, surfacevals & hv ) const;

  vector3d centroid() const
  { 
    return vector3d(0,0,0); 
  }
  
  dynarray<vector3d> elements() const
  {
    return dynarray<vector3d>();
  }

  boundingbox box;
  surface * left;
  surface * right;

 private:
  hbv_node();  // no default constructing.
};

#endif

























