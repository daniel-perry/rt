/* Daniel Perry
 * for cs6620 - spring 2005
 * started 22mar05
 */

/* BBox: bounding box to be used in a bounding volume hierarchy acceleration structure.
 */

#ifndef _BBOX_H
#define _BBOX_H

#include "vector3d.h"
#include "Object.h"

class BBox : public Object
{
 public:
  BBox();
  BBox( const Point & min_pt , const Point & max_pt );

  void preprocess();
  bool intersect( HitRecord & hit , const RenderContext & context , const ray & r );

  BBox getBBox();

  Point centroid();

  char *  print(){return "BBox";}

  Point bounds[2];
};

BBox merge ( const BBox & b1 , const BBox & b2 );

#endif
