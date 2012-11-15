/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 */


/* Sphere: basic sphere class.
 *
 *
 */
#include "util.h" //for min and max float vals.
#include "Sphere.h"
#include "ray.h"
#include "BBox.h"

bool Sphere::intersect( HitRecord & hit , const RenderContext & context, const ray & r ) {
  // for now using function defined in ray.h,ray.cc
  double t;
  if( ray_sphere_intersect( r, center, radius, MYMIN, MYMAX, t ) ){
    return hit.hit( t , this , material );
  }
  return false;
}

BBox Sphere::getBBox(){
  Vector rad( radius , radius , radius );

  return BBox( center - rad , center + rad );
}
