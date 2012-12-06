/* Daniel Perry
 * for cs6620 - spring 2005
 * started 7feb05
 */


/* Disk: basic disk class.
 *
 *
 */

#include "Disk.h"
#include "util.h"
#include <math.h>

bool Disk::intersect( HitRecord & hit , const RenderContext & context, const ray & r ){
  double cosTheta = dot( r.direction() , Normal );
  if( fabs( cosTheta) > MYMIN){ // NOT parallel to plane in which disk resides
    
    double t = (d - dot( r.origin() , Normal)) / cosTheta;
    
    // Now know that it hit's the same plane and where, need to determin if it lies within the disk.
    vector3d intersect_pt = r.eval( t );
    
    vector3d dist = intersect_pt - center;

    if( dist.squaredLength() < radius * radius ){
      return hit.hit( t , Normal, this , material );
    }    
  }
  return false; // parallel to plane in which disk resides or hit plane outside disk
}

BBox Disk::getBBox(){
  BBox bbox;
  double x_offset = radius * sqrt( Normal.y()*Normal.y() + Normal.z()*Normal.z() ),
         y_offset = radius * sqrt( Normal.x()*Normal.x() + Normal.z()*Normal.z() ),
         z_offset = radius * sqrt( Normal.x()*Normal.x() + Normal.y()*Normal.y() );
  Vector offset( x_offset , y_offset , z_offset );

  bbox.bounds[0] = (center - offset);
  bbox.bounds[1] = (center + offset);
  return bbox;
}
