#include "axisbox.h"

axisbox::axisbox()
{
  setp0( vector3d(0,0,0) );
  setp1( vector3d(1,1,1) );
  setmatte( rgb(1,1,1) );
  setmirror( rgb(0,0,0) );
  sethighlight( 0.0 );
  setphongexponent( 1.0 );
}

axisbox::axisbox( const vector3d & np0, const vector3d & np1, const rgb & kd, const rgb & ks, double kh, double e )
{
  setp0( np0 );
  setp1( np1 );
  setmatte( kd );
  setmirror( ks );
  sethighlight( kh );
  setphongexponent( e );
}

bool axisbox::hit (const ray & r, double tmin, double tmax, double & t, surfacevals & sv ) const
{
  if ( ray_box_intersect( r , p0() , p1() , tmin , tmax , sv.normal , t ) )
  {
    sv.matte = matte();
    sv.mirror = mirror();
    sv.highlight = highlight();
    sv.phongexponent = phongexponent();
    sv.normal /= sv.normal.length();
    return true;
  }
  
  return false;
}

vector3d axisbox::centroid() const
{
  return ( p0() + p1() ) / 2 ;
}

dynarray<vector3d> axisbox::elements() const
{
  dynarray<vector3d> myvarray;
  myvarray.append(p0());
  myvarray.append(p1());
  return myvarray;
}

dynarray<triangle> axisbox::triangulate() const
{
  dynarray<triangle> tri_array;

  vector3d leftTopFront(p0().x(),p1().y(),p0().z()), rightTopFront(p1().x(),p1().y(),p0().z()), rightBottomFront(p1().x(),p0().y(),p0().z()),
           leftTopBack(p0().x(),p1().y(),p1().z()), leftBottomBack(p0().x(),p0().y(),p1().z()), rightBottomBack(p1().x(),p0().y(),p1().z());

  //front
  tri_array.append(triangle(p0(),leftTopFront,rightTopFront,matte(),mirror(),highlight(),phongexponent(),myTexture));
  tri_array.append(triangle(p0(),rightBottomFront,rightTopFront,matte(),mirror(),highlight(),phongexponent(),myTexture));
  //left side
  tri_array.append(triangle(p0(),leftBottomBack,leftTopFront,matte(),mirror(),highlight(),phongexponent(),myTexture));
  tri_array.append(triangle(leftTopBack,leftBottomBack,leftTopFront,matte(),mirror(),highlight(),phongexponent(),myTexture));
  //right side
  tri_array.append(triangle(rightBottomFront,rightBottomBack,rightTopFront,matte(),mirror(),highlight(),phongexponent(),myTexture));
  tri_array.append(triangle(p1(),rightTopFront,rightBottomBack,matte(),mirror(),highlight(),phongexponent(),myTexture));
  //top
  tri_array.append(triangle(leftTopFront,rightTopFront,p1(),matte(),mirror(),highlight(),phongexponent(),myTexture));
  tri_array.append(triangle(leftTopBack,p1(),leftTopFront,matte(),mirror(),highlight(),phongexponent(),myTexture));
  //bottom
  tri_array.append(triangle(p0(),rightBottomFront,rightBottomBack,matte(),mirror(),highlight(),phongexponent(),myTexture));
  tri_array.append(triangle(p0(),leftBottomBack,rightBottomBack,matte(),mirror(),highlight(),phongexponent(),myTexture));
  //back
  tri_array.append(triangle(p1(),rightBottomBack,leftBottomBack,matte(),mirror(),highlight(),phongexponent(),myTexture));
  tri_array.append(triangle(p1(),leftTopBack,leftBottomBack,matte(),mirror(),highlight(),phongexponent(),myTexture));
  
  //triangle tritest(p0(),rightBottomFront,rightBottomBack,matte());

  return tri_array;
}
