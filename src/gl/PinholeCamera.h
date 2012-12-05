/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 */

/* PinholeCamera: camera implementation, modeling a pinhole viewer.
 *
 *
 */


#ifndef _PINHOLECAMERA_H
#define _PINHOLECAMERA_H

#include <string>
#include <sstream>

#include "Camera.h"
#include "hpoint.h"
#include "vector3d.h"
#include "RenderContext.h"

class PinholeCamera : public Camera
{
 public:
  PinholeCamera( const hpoint & pos , const vector3d & g , const vector3d & up , double theta, double AR);
  PinholeCamera( const hpoint & eye , const hpoint & look_at, const vector3d & up ,  double theta , double AR );

  PinholeCamera( const vector3d & eye , const vector3d & look_at, const vector3d & nup , double ntheta, double AR );

  // below, x,y = [-1,1].
  void makeRay ( ray & r, const RenderContext & context, double x , double y ) const;

  void preprocess(double aspect_ratio);

  void initialize(const vector3d & eye , const vector3d & look_at, const vector3d & nup , double ntheta, double AR );

  virtual std::string toString()
  {
    std::stringstream s;
    s << "pinhole pos="<<position<<", gaze="<<gaze<<", up="<<up;
    return s.str();
  }

  hpoint position;
  vector3d gaze;
  vector3d up;
  double theta;
  // derived:
  vector3d u,v;

 private:  
  PinholeCamera(){}

};


#endif
