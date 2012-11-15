#include "boundingbox.h"
#include "ray.h"

bool boundingbox::hit( const ray & r, double tmin, double tmax ) const
{
  return ray_bbox_intersect( r, point1(), point2(), tmin, tmax);
}

ostream & operator << ( ostream & os , const boundingbox & box )
{
  os << "[ " << box.point1() << " , " << box.point2() << " ]";
  return os;
}

bool ray_bbox_intersect(const ray& r, const vector3d& p0, const vector3d& p1, double ta_min, double ta_max)
{

      double tx_min, tx_max;
      double ty_min, ty_max;
      double tz_min, tz_max;
 
      double t_min, t_max;

      if (r.direction().x() >= 0) {
           tx_min = (p0.x() - r.origin().x()) / r.direction().x();
           tx_max = (p1.x() - r.origin().x()) / r.direction().x();
      }
      else {
           tx_min = (p1.x() - r.origin().x()) / r.direction().x();
           tx_max = (p0.x() - r.origin().x()) / r.direction().x();
      }

      if (r.direction().y() >= 0) {
           ty_min = (p0.y() - r.origin().y()) / r.direction().y();
           ty_max = (p1.y() - r.origin().y()) / r.direction().y();
      }
      else {
           ty_min = (p1.y() - r.origin().y()) / r.direction().y();
           ty_max = (p0.y() - r.origin().y()) / r.direction().y();
      }

      if (tx_min < ty_max && ty_min < tx_max) { // x and y overlap
           if (tx_min < ty_min) {
               t_min = ty_min;
           }
           else {
               t_min = tx_min;
           }
           if (tx_max > ty_max) {
               t_max = ty_max;
           }
           else {
               t_max = tx_max;
           }
      }
      else
          return false;

      if (r.direction().z() >= 0) {
           tz_min = (p0.z() - r.origin().z()) / r.direction().z();
           tz_max = (p1.z() - r.origin().z()) / r.direction().z();
      }
      else {
           tz_min = (p1.z() - r.origin().z()) / r.direction().z();
           tz_max = (p0.z() - r.origin().z()) / r.direction().z();
      }

      if (t_min < tz_max && tz_min < t_max) { // xy and z overlap
           if (t_min < tz_min) {
               t_min = tz_min;
           }
           if (t_max > tz_max) {
               t_max = tz_max;
           }
      }
      else
          return false;

      if (t_min >= ta_min && t_min <= ta_max) {
           return true;
      }  
      else if (t_max >= ta_min && t_max <= ta_max) {
           return true;
      }
      else
          return false;

}


