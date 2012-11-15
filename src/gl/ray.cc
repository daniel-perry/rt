/* Daniel Perry
 * originally written in fall 2001 for cs3505
 *
 */

#include "ray.h"

ostream & operator << ( ostream & os , ray r )
{
  os << "o=" << r.origin() << ", d=" << r.direction();
  return os;
}
// ( ray_to_check, center , radius, tmin, tmax, t);
// eg: ( ray( (0,0,0) (1,0,0)), (3,0,0), .5 , 0 , 100, t(returns value of t at intersection) )
bool ray_sphere_intersect(const ray& r, const vector3d& c, double radius, double tmin, double tmax, double& t)
{
  // At^2 + Bt + C
  double A = r.direction().squaredLength(),
         B = 2 * dot( r.direction() , r.origin() - c ),
         C = (r.origin() - c).squaredLength() - radius*radius,
         discriminant, ttemp;
  
  if ( (discriminant = B*B-4*A*C) >=0 )
  {
    discriminant = sqrt( discriminant );

    ttemp = (-B - discriminant) / (2*A);
    if ( ttemp > tmin && ttemp < tmax )
    {
      t = ttemp;
      return true;
    }

    ttemp = (-B + discriminant) / (2*A);
    if ( ttemp > tmin && ttemp < tmax )
    {
      t = ttemp;
      return true;
    }
    
    return false;
  }
 
  return false;
}



bool ray_triangle_intersect(const ray& r,
			    const vector3d& p0, const vector3d& p1, const vector3d& p2,
			    double tmin, double tmax, double& beta, double& gamma, double& t)
{
  double a = p0.x() - p1.x(), b = p0.y() - p1.y(), c = p0.z() - p1.z(),
         d = p0.x() - p2.x(), e = p0.y() - p2.y(), f = p0.z() - p2.z(),
         g = r.direction().x(), h = r.direction().y(), i = r.direction().z(),
         j = p0.x() - r.origin().x(), k = p0.y() - r.origin().y(), l = p0.z() - r.origin().z(),

         M = 1 / (a*(e*i-h*f) + b*(g*f-d*i) + c*(d*h-e*g)),
         ttemp = ( f*(j*b-a*k) + e*(a*l-j*c) + d*(k*c-b*l) ) * M,
         tbeta, tgamma;

  ////////////////////////////////////////
  // DEBUG
  /*
  double ttgamma =  i*(a*k-j*b) + h*(j*c-a*l) + g*(b*l-k*c), 
         ttbeta = j*(e*i-h*f) + k*(g*f-d*i) + l*(d*h-e*g);
  cout<< "ttemp:" << ttemp << endl
      << "tgamma:" << ttgamma * M << endl
      << "tbeta:" <<  ttbeta * M << endl << endl;

  cout<<"[ "<<a<<" "<<d<<" "<<g<<" ] [B]   "<<"[ "<<j<<" ]"<<endl
      <<"[ "<<b<<" "<<e<<" "<<h<<" ] [G] = "<<"[ "<<k<<" ]"<<endl
      <<"[ "<<c<<" "<<f<<" "<<i<<" ] [T]   "<<"[ "<<l<<" ]"<<endl<<endl;

  cout<< "Dttemp:" << ttemp/M << endl
      << "Dtgamma:" << ttgamma << endl
      << "Dtbeta:" <<  ttbeta  << endl
      << "D:"<< 1/M  << endl << endl;
    */
  //
  //////////////////////////////////////////////////////////////

  if ( ttemp <= tmin || ttemp >= tmax )
    return false;
  
  tgamma = ( i*(a*k-j*b) + h*(j*c-a*l) + g*(b*l-k*c) ) * M;
  if ( tgamma < 0 || tgamma > 1 )
    return false;
  
  tbeta = ( j*(e*i-h*f) + k*(g*f-d*i) + l*(d*h-e*g) ) * M;
  if ( tbeta < 0 || tbeta > (1-tgamma) )
    return false;

  beta = tbeta;
  gamma = tgamma;
  t = ttemp;
  return true;
}



// for an axis-aligned box
// from Pete Shirley's code
bool ray_box_intersect(const ray& r, const vector3d& p0, const vector3d& p1, double ta_min, double ta_max, vector3d& n, double& t)
{

      double tx_min, tx_max;
      double ty_min, ty_max;
      double tz_min, tz_max;
      vector3d nx_min, nx_max;
      vector3d ny_min, ny_max;
      vector3d nz_min, nz_max;
      vector3d n_min, n_max;
      double t_min, t_max;

      if (r.direction().x() >= 0) {
           tx_min = (p0.x() - r.origin().x()) / r.direction().x();
           tx_max = (p1.x() - r.origin().x()) / r.direction().x();
           nx_min = vector3d(-1,0,0);
           nx_max = vector3d( 1,0,0);
      }
      else {
           tx_min = (p1.x() - r.origin().x()) / r.direction().x();
           tx_max = (p0.x() - r.origin().x()) / r.direction().x();
           nx_min = vector3d( 1,0,0);
           nx_max = vector3d(-1,0,0);
      }

      if (r.direction().y() >= 0) {
           ty_min = (p0.y() - r.origin().y()) / r.direction().y();
           ty_max = (p1.y() - r.origin().y()) / r.direction().y();
           ny_min = vector3d(0,-1,0);
           ny_max = vector3d( 0,1,0);
      }
      else {
           ty_min = (p1.y() - r.origin().y()) / r.direction().y();
           ty_max = (p0.y() - r.origin().y()) / r.direction().y();
           ny_min = vector3d(0,1,0);
           ny_max = vector3d(0,-1,0);
      }

      if (tx_min < ty_max && ty_min < tx_max) { // x and y overlap
           if (tx_min < ty_min) {
               t_min = ty_min;
               n_min = ny_min;
           }
           else {
               t_min = tx_min;
               n_min = nx_min;
           }
           if (tx_max > ty_max) {
               t_max = ty_max;
               n_max = ny_max;
           }
           else {
               t_max = tx_max;
               n_max = nx_max;
           }
      }
      else
          return false;

      if (r.direction().z() >= 0) {
           tz_min = (p0.z() - r.origin().z()) / r.direction().z();
           tz_max = (p1.z() - r.origin().z()) / r.direction().z();
           nz_min = vector3d(0,0,-1);
           nz_max = vector3d(0,0, 1);
      }
      else {
           tz_min = (p1.z() - r.origin().z()) / r.direction().z();
           tz_max = (p0.z() - r.origin().z()) / r.direction().z();
           nz_min = vector3d(0,0, 1);
           nz_max = vector3d(0,0,-1);
      }

      if (t_min < tz_max && tz_min < t_max) { // xy and z overlap
           if (t_min < tz_min) {
               t_min = tz_min;
               n_min = nz_min;
           }
           if (t_max > tz_max) {
               t_max = tz_max;
               n_max = nz_max;
           }
      }
      else
          return false;

      if (t_min >= ta_min && t_min <= ta_max) {
           t = t_min;
           n = n_min;
           return true;
      }  
      else if (t_max >= ta_min && t_max <= ta_max) {
           t = t_max;
           n = n_max;
           return true;
      }
      else
          return false;

}


