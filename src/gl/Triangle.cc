/* Daniel Perry
 * for cs6620 - spring 2005
 * started 7feb05
 */


/* Triangle: basic triangle class.
 *
 *
 */

#include "Triangle.h"
#include "ray.h"
#include "util.h"
#include "BBox.h"
#include <math.h>

BBox Triangle::getBBox(){
  Point min( fmin( p0.x() , fmin( p1.x() , p2.x() )) , fmin( p0.y() , fmin( p1.y() , p2.y() )) , fmin( p0.z() , fmin( p1.z() , p2.z() )) ),
    max (  fmax( p0.x() , fmax( p1.x() , p2.x() )) , fmax( p0.y() , fmax( p1.y() , p2.y() )) , fmax( p0.z() , fmax( p1.z() , p2.z() )) );

  return BBox( min , max );
}

bool Triangle::intersect(  HitRecord & hit , const RenderContext & context, const ray & r ){
  //  cerr<<"triangle intersect"<<endl;
  double t, beta, gamma;

  if( ray_triangle_intersect( r , p0, p1, p2, MYMIN , MYMAX, beta, gamma , t ) ){
    return hit.hit( t, Normal, this , material );
  }
  return false;

}
