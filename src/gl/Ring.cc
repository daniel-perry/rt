/* Daniel Perry
 * for cs6620 - spring 2005
 * started 7feb05
 */


/* Ring: basic Ring class.
 *
 *
 */

#include "Ring.h"
#include <math.h>
#include "BBox.h"

bool Ring::intersect( HitRecord & hit , const RenderContext & context, const ray & r ) const{
  double cosTheta = dot( r.direction() , Normal );
  if( fabs( cosTheta) > MYMIN){ // NOT parallel to plane in which disk resides
    
    double t = (d - dot( r.origin() , Normal)) / cosTheta;
    
    // Now know that it hit's the same plane and where, need to determin if it lies within the disk.
    vector3d intersect_pt = r.eval( t );
    
    vector3d dist = intersect_pt - center;
    double distSq = dist.squaredLength();
    if( distSq < largeRadius*largeRadius && distSq > smallRadius*smallRadius ){
      return hit.hit( t, Normal, this , material );
    }    
  }
  return false; // parallel to plane in which disk resides or hit plane outside disk
}


BBox Ring::getBBox(){
  BBox bbox;
  double x_offset = largeRadius * sqrt( Normal.y()*Normal.y() + Normal.z()*Normal.z() ),
         y_offset = largeRadius * sqrt( Normal.x()*Normal.x() + Normal.z()*Normal.z() ),
         z_offset = largeRadius * sqrt( Normal.x()*Normal.x() + Normal.y()*Normal.y() );
  Vector offset( x_offset , y_offset , z_offset );

  bbox.bounds[0] = (center - offset);
  bbox.bounds[1] = (center + offset);
  return bbox;
}
