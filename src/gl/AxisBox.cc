/* Daniel Perry
 * for cs6620 - spring 2005
 * started 7feb05
 */


/* AxisBox: basic Axis-aligned box class.
 *
 *
 */

#include "AxisBox.h"
#include "util.h"

// using algorithm by Amy Williams found at http://www.cs.utah.edu/~awilliam/box/box.pdf
bool AxisBox::intersect( HitRecord & hit , const RenderContext & context, const ray & r ){
  
  double t0 = MYMIN , t1 = MYMAX;
  double tmin,tmax,tymin,tymax,tzmin,tzmax;
  vector3d n_min,n_max;

  tmin = (bounds[r.sign[0]].x() - r.origin().x()) * r.inv_d.x();
  tmax = (bounds[1-r.sign[0]].x() - r.origin().x()) * r.inv_d.x();
  tymin = (bounds[r.sign[1]].y() - r.origin().y()) * r.inv_d.y();
  tymax = (bounds[1-r.sign[1]].y() - r.origin().y()) * r.inv_d.y();

  if( (tmin>tymax) || (tymin > tmax) ){
    return false;
  }
  if( tymin > tmin ){
    tmin = tymin;
      n_min = vector3d(0,-1,0);
  }else{
      n_min = vector3d(-1,0,0);
  }
  if( tymax < tmax ){
    tmax = tymax;
    n_max = vector3d(0,1,0);
  }else{
    n_max = vector3d(1,0,0);
  }
  tzmin = (bounds[r.sign[2]].z() - r.origin().z()) * r.inv_d.z();
  tzmax = (bounds[1-r.sign[2]].z() - r.origin().z()) * r.inv_d.z();
  if( (tmin > tzmax) || (tzmin > tmax) ){
    return false;
  }
  if( tzmin > tmin ){
    tmin = tzmin;
    n_min = vector3d(0,0,-1);
  }
  if( tzmax < tmax ){
    tmax = tzmax;
    n_max = vector3d(0,0,1);
  }

  if( (tmin < t1 ) && (tmax > t0) ){
    vector3d checker(r.sign[0],r.sign[1],r.sign[2]); // to reverse normals if ray approaches with negative slope.
    checker*=-1;
    checker += vector3d( 1-r.sign[0], 1-r.sign[1], 1-r.sign[2] ); // should end up with checker looking like (1,-1,1) depending on whether slope was negative or positive in (x,y,z)

    // ie : checker = <1,1,1> - 2 * <sign>
    n_min *= checker;
    n_max *= checker;

    Normal = n_min;

    if(tmin < MYMIN ){ // if in center of box, this will return the one infront....
      tmin = tmax;
      Normal = n_max;
    }

    return hit.hit( tmin , this , material ) ;
  }
  return false;

}
