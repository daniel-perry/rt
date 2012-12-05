/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 */

/* PinholeCamera: camera implementation, modeling a pinhole viewer.
 *
 *
 *
 */


#include "PinholeCamera.h"
#include <math.h>

//default camera:
/*
PinholeCamera::PinholeCamera(){
  position = hpoint(0,0,0,0);
  gaze = vector3d( 0,0,1);
  up = vector3d( 0,1,0);
  theta = 0.78539816; // (approx for  Pi/4 )

  preprocess(1); // derive u,v,gazeNormal.
}
PinholeCamera::PinholeCamera( const hpoint & pos , const vector3d & g , const vector3d & nup , double nTheta, double aspectRatio){

  position = pos;
  gaze = g;
  up = nup;
  theta = nTheta;

  preprocess(aspectRatio); // derive u,v,gazeNormal.
  
}*/

PinholeCamera::PinholeCamera( const hpoint & eye , const hpoint & look_at, const vector3d & nup ,  double ntheta , double AR ){
  position = eye;
  vector3d e(eye.x(),eye.y(),eye.z()), la(look_at.x(),look_at.y(),look_at.z());
  gaze = la-e;
  up = nup;
  theta = M_PI/180 * ntheta;
  preprocess( AR);
}

PinholeCamera::PinholeCamera( const vector3d & eye , const vector3d & look_at, const vector3d & nup , double ntheta, double AR ){
  initialize(eye,look_at,nup,ntheta,AR);
}

void PinholeCamera::initialize( const vector3d & eye , const vector3d & look_at, const vector3d & nup , double ntheta, double AR )
{
  position = hpoint(eye.x(),eye.y(),eye.z());
  vector3d e=eye, la=look_at;
  gaze = la-e;
  up = nup;
  theta = M_PI/180 * ntheta;
  preprocess( AR);
}

void PinholeCamera::preprocess( double aspect_ratio ){
  //g.MakeUnitVector();
  u = cross( gaze , up );
  v = cross( u , gaze );
  u.MakeUnitVector();
  double tanTheta = tan( theta/2 );
  u *= tanTheta ;
  v.MakeUnitVector();
  v *= (tanTheta/aspect_ratio);
  gaze.MakeUnitVector();

}

void PinholeCamera::makeRay ( ray & r, const RenderContext & context, double x , double y ) const{
  r.setorigin( vector3d(position.x(),position.y(),position.z()) );
  r.setdirection( gaze + (x*u) + (y*v) );
}
