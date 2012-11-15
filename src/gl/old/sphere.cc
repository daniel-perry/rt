#include "sphere.h"
#include "light.h"

 sphere::sphere()
  {
    setradius( 1 );
    setcenter( vector3d(0,0,0) );
    setmatte( rgb(1,1,1) );
    setmirror( rgb(0,0,0) );
    sethighlight( 0.0 );
    setphongexponent( 1 );
  }
  sphere::sphere( const vector3d & c, double r, const rgb & kd, const rgb & ks, double kh, double e )
  {
    setradius( r );
    setcenter( c );
    setmatte( kd );
    setmirror( ks );
    sethighlight( kh );
    setphongexponent( e );
  }
 
  bool sphere:: hit (const ray & r, double tmin, double tmax, double & t, surfacevals & sv ) const
  {
    if ( ray_sphere_intersect( r , center() , radius() , tmin, tmax, t ) )
    {
      sv.matte = matte();
      sv.mirror = mirror();
      sv.highlight = highlight();
      sv.normal = normal( r.eval(t) );
      sv.phongexponent = phongexponent();
      return true;
    }
    
    return false;
  }

vector3d sphere::centroid() const
{
  return center();
}

// since this is a sphere, just the center wouldn't be usefull, so I am returning the center translated 6 ways (twice in each x,y,z direction) to give the "limits" of the sphere
dynarray<vector3d> sphere::elements() const
{
  dynarray<vector3d> myArray;
  myArray.append(center()+vector3d(radius(),0,0));
  myArray.append(center()+vector3d(-radius(),0,0));
  myArray.append(center()+vector3d(0,radius(),0));
  myArray.append(center()+vector3d(0,-radius(),0));
  myArray.append(center()+vector3d(0,0,radius()));
  myArray.append(center()+vector3d(0,0,-radius()));

  return myArray;
}





