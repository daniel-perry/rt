#include "triangle.h"

bool triangle:: hit (const ray & r, double tmin, double tmax, double & t, surfacevals & sv ) const
{
  double beta,gamma;
  if ( ray_triangle_intersect( r , p0() , p1() , p2() , tmin , tmax , beta , gamma , t ) )
  {
    sv.matte = matte();
    sv.mirror = mirror();
    sv.highlight = highlight();
    sv.phongexponent = phongexponent();
    sv.normal = normal();
    return true;
  }
  
  return false;
}

vector3d triangle::centroid() const
{
  return ( p0() + p1() + p2() )/3;
}

dynarray<vector3d> triangle::elements() const
{
  dynarray<vector3d> myArray;
  myArray.append( p0() );
  myArray.append( p1() );
  myArray.append( p2() );

  return myArray;
}

