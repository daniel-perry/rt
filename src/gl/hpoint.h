/* Daniel Perry
 * originally written in fall 2001 for cs3505
 *
 */

#ifndef _HPOINT_H
#define _HPOINT_H

#include <cmath>
#include <cassert>
#include <iostream>
#include "vector3d.h"
#include "matrix.h"

#include <values.h> //for MINFLOAT

#include "util.h" //for uniform min and max values.

#define MY_EPSILON  MYMIN //MINFLOAT //for checking for divide by zero w/ floats

using std::cerr;
using std::endl;
using std::istream;
using std::ostream;

class hpoint
{
 public:
  hpoint( double p0=0 , double p1=0 , double p2=0 , double p3=1 ){ data[0]=p0; data[1]=p1; data[2]=p2; data[3]=p3; }

  //hpoint( const vector3d & v ){ data[0]=v.x(); data[1]=v.y(); data[2]=v.z(); data[3]=0; }

  hpoint( const vector3d & v , double hval ){ data[0]=v.x(); data[1]=v.y(); data[2]=v.z(); data[3]=hval; }

  hpoint( const hpoint & h ){ data[0]=h.x(); data[1]=h.y(); data[2]=h.z(); data[3]=h.h(); }

  void set( const vector3d & v , double hval ){ data[0]=v.x(); data[1]=v.y(); data[2]=v.z(); data[3]=hval; }

  vector3d tovector3d()
  {
    return vector3d(x(),y(),z());
  }

  double x() const { return data[0]; }
  int xint() const { return (int)(data[0]>=0 ? floor(data[0]) : ceil(data[0])); }
  double y() const { return data[1]; }
  int yint() const { return (int)(data[1]>=0 ? floor(data[1]) : ceil(data[1])); }
  double z() const { return data[2]; }
  int zint() const { return (int)(data[2]>=0 ? floor(data[2]) : ceil(data[2])); }
  double h() const { return data[3]; }
  int hint() const { return (int)(data[3]>=0 ? floor(data[3]) : ceil(data[3])); }
 
  void setx( double x ) { data[0]=x; }
  void sety( double y ) { data[1]=y; }
  void setz( double z ) { data[2]=z; }
  void seth( double h ) { data[3]=h; }

  const hpoint & operator+() const{ return (*this); }
  hpoint operator-() const { return hpoint( -x() , -y() , -z() , -h() ); }
  double operator[](int i) const { assert( i >= 0 && i < 4 ); return data[i]; }
  double & operator[](int i) { assert( i >= 0 && i < 4 ); return data[i]; }

  hpoint & operator+=(const hpoint &v2) { data[0]+=v2.x(); data[1]+=v2.y(); data[2]+=v2.z(); data[3]+=v2.h(); return (*this); }
  hpoint & operator-=(const hpoint &v2) { return (*this)+=(-v2); }
  hpoint & operator*=(const double t) { data[0]*=t; data[1]*=t; data[2]*=t; data[3]*=t; return (*this); }
  hpoint & operator/=(const double t) { 
    if ((t<MY_EPSILON && t>-MY_EPSILON ))
    {
      cerr<<"hpoint: div by zero, t="<<t<<endl;
    }
    assert( !(t<MY_EPSILON && t>-MY_EPSILON ) ); 
    return (*this)*=(1/t); 
  }  // checking for t==0
  //hpoint & operator/=(const double t) { return (*this)*=(1/t); }  // checking for t==0

  hpoint & homogenize(){ return (*this)/=h(); }
  vector3d tovector3d() const { return vector3d(x(),y(),z()); }
  // 3-space distance
  double distance( const hpoint & h = hpoint(0,0,0,0) ) { return sqrt( (x()-h.x())*(x()-h.x())+(y()-h.y())*(y()-h.y())+(z()-h.y())*(z()-h.z()) ); }
 private:
  double data[4];
};

inline bool operator==(const hpoint &t1, const hpoint &t2) { return ( t1.x()==t2.x() && t1.y()==t2.y() && t1.z()==t2.z() && t1.h()==t2.h() ); }
inline bool operator!=(const hpoint &t1, const hpoint &t2) { return !(t1==t2); }

//!!!!!!!!!ONLY INPUTING THREE VALS WITH OPERATOR >>!!!!!
inline istream &operator>>(istream &is, hpoint &t){ return (is>>t[0]>>t[1]>>t[2]); }//{ return (is>>t[0]>>t[1]>>t[2]>>t[3]); }
inline ostream &operator<<(ostream &os, const hpoint &t){ return (os<<"("<<t.x()<<","<<t.y()<<","<<t.z()<<","<<t.h()<<")"); }


inline hpoint operator+(const hpoint &v1, const hpoint &v2){ return hpoint( v1.x()+v2.x() , v1.y()+v2.y() , v1.z()+v2.z() , v1.h()+v2.h() ); }
inline hpoint operator-(const hpoint &v1, const hpoint &v2){ return v1+(-v2); }
inline hpoint operator*(double t, const hpoint &v){ return hpoint( v.x()*t , v.y()*t , v.z()*t , v.h()*t ); }
inline hpoint operator*(const hpoint &v, double t){ return t*v; }
//inline hpoint operator/(const hpoint &v, double t){ assert( !(t<MY_EPSILON && t>-MY_EPSILON ) ); return (1/t)*v; }
inline hpoint operator/(const hpoint &v, double t){ hpoint temp(v); temp/=t; return temp; }


inline hpoint operator*(const matrix & m , const hpoint & p )
{
  assert( m.cols()==4 && m.rows()==4 );
  return hpoint( m.get(0,0)*p.x()+m.get(0,1)*p.y()+m.get(0,2)*p.z()+m.get(0,3)*p.h(),
		 m.get(1,0)*p.x()+m.get(1,1)*p.y()+m.get(1,2)*p.z()+m.get(1,3)*p.h(),
		 m.get(2,0)*p.x()+m.get(2,1)*p.y()+m.get(2,2)*p.z()+m.get(2,3)*p.h(),
		 m.get(3,0)*p.x()+m.get(3,1)*p.y()+m.get(3,2)*p.z()+m.get(3,3)*p.h() );
}



// these functions treat hpoints as 3d vectors (ignore the 4th coord)
inline hpoint cross( const hpoint & v1 , const hpoint & v2 ){ return hpoint( v1.y()*v2.z() - v1.z()*v2.y() , v1.z()*v2.x() - v1.x()*v2.z() , v1.x()*v2.y() - v1.y()*v2.x(), 0);}
inline double dot(const hpoint &v1, const hpoint &v2){ return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z(); }

#endif

