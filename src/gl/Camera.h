/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 */

/* Camera: superclass of different types of cameras
 *
 * direct subclasses: PinholeCamera, OrthographicCamera
 *
 */

#ifndef _CAMERA_H
#define _CAMERA_H

using namespace std;

class RenderContext;
//#include "RenderContext.h"
#include "ray.h"

class Camera
{
 public:
  virtual void preprocess( double aspect_ratio) = 0;
  //virtual void preprocess() = 0;
  // to return a ray for tracing for that point on the "film"
  virtual void makeRay ( ray & r , const RenderContext & context, double x , double y ) const = 0;
  
  virtual std::string toString() = 0;
};

#endif
