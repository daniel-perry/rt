#include "vector3d.h"

#include <math.h>
#include <assert.h>
#include <iostream.h>

/*   
///////////////////////////////
vector3d::vector3d()
{
  setx(0.0);
  sety(0.0);
  setz(0.0);
}
vector3d::vector3d(double e0, double e1, double e2) 
{
  setx(e0);
  sety(e1);
  setz(e2);
}
vector3d::vector3d(const vector3d &v)
{
  setx(v.x());
  sety(v.y());
  setz(v.z());
}

double vector3d::x() const 
{ 
  return data[0]; 
}
double vector3d::y() const 
{ 
  return data[1]; 
}
double vector3d::z() const 
{ 
  return data[2]; 
}
 
void vector3d::setx( double x ) 
{ 
  data[0]=x; 
}
void vector3d::sety( double y ) 
{ 
  data[1]=y; 
}
void vector3d::setz( double z ) 
{ 
  data[2]=z; 
}
////////////////////////////
*/


const vector3d& vector3d::operator+() const 
{ 
  return (*this); 
}
vector3d vector3d::operator-() const 
{ 
  return vector3d( -x(), -y(), -z() ); 
}
double vector3d::operator[](int i) const 
{ 
  assert(i>=0 && i<3); 
  return data[i];  
}
double& vector3d::operator[](int i) 
{ 
  assert(i>=0 && i<3); 
  return data[i]; 
}

vector3d& vector3d::operator+=(const vector3d &v2) 
{ 
  setx( x()+v2.x() ); 
  sety( y()+v2.y() ); 
  setz( z()+v2.z() );
  return (*this);
}
vector3d& vector3d::operator-=(const vector3d &v2) 
{ 
  setx( x()-v2.x() ); 
  sety( y()-v2.y() ); 
  setz( z()-v2.z() ); 
  return (*this);
}
vector3d& vector3d::operator*=(const double t) 
{ 
  setx( x()*t ); 
  sety( y()*t ); 
  setz( z()*t ); 
  return (*this);
}
vector3d& vector3d::operator/=(const double t)  // note: not checking for div by zero .
{ 
  setx( x()/t ); 
  sety( y()/t ); 
  setz( z()/t );
  return (*this);
} 

double vector3d::length() const 
{ 
  return sqrt( x()*x() + y()*y() + z()*z() ); 
}
double vector3d::squaredLength() const 
{ 
  return x()*x() + y()*y() + z()*z(); 
}

void vector3d::MakeUnitVector() 
{ 
  (*this)/=length(); 
}


int vector3d::indexOfMinComponent() const
{
  if( x() <= y() )
    if( x() <= z() )
      return 0;
    else
      return 2;
  else if ( y() <= z() )
    return 1;
  return 2;
}

int vector3d::indexOfMinAbsComponent() const
{
  double xv=fabs(x()),yv=fabs(y()),zv=fabs(z());
  if( xv <= yv )
    if( xv <= zv )
      return 0;
    else
      return 2;
  else if ( yv <= zv )
    return 1;
  return 2;
}

int vector3d::indexOfMaxComponent() const
{
  if( x() >= y() )
    if( x() >= z() )
      return 0;
    else
      return 2;
  else if ( y() >= z() )
    return 1;
  return 2;
}

int vector3d::indexOfMaxAbsComponent() const
{
  double xv=fabs(x()),yv=fabs(y()),zv=fabs(z());
  if( xv >= yv )
    if( xv >= zv )
      return 0;
    else
      return 2;
  else if ( yv >= zv )
    return 1;
  return 2;
}
double vector3d::minComponent() const 
{ 
  return (*this)[ indexOfMinComponent() ]; 
}
double vector3d::maxComponent() const 
{ 
  return (*this)[ indexOfMaxComponent() ]; 
}
double vector3d::maxAbsComponent() const 
{ 
  return fabs( (*this)[ indexOfMaxAbsComponent() ] ); 
}
double vector3d::minAbsComponent() const 
{ 
  return fabs( (*this)[ indexOfMinAbsComponent() ] ); 
}

///////////////////////////
// NON-MEMBER FUNCTIONS
///////////////////////////
bool operator==(const vector3d &t1, const vector3d &t2)
{
  if (t1.x() == t2.x() && t1.y() == t2.y() && t1.z() == t2.z() )
    return true;
  return false;
}
bool operator!=(const vector3d &t1, const vector3d &t2)
{
  return !(t1==t2);
}

istream &operator>>(istream &is, vector3d &t)
{
  is>>t[0]>>t[1]>>t[2];
  return is;
}
ostream &operator<<(ostream &os, const vector3d &t)
{
  os<<'('<<t.x()<<','<<t.y()<<','<<t.z()<<')';
  return os;
}

vector3d operator+(const vector3d &v1, const vector3d &v2)
{
  return vector3d( v1.x()+v2.x() , v1.y()+v2.y() , v1.z()+v2.z() );
}
vector3d operator-(const vector3d &v1, const vector3d &v2)
{
  return vector3d( v1.x()-v2.x() , v1.y()-v2.y() , v1.z()-v2.z() );
}

vector3d operator*(double t, const vector3d &v)
{
  return vector3d( t*v.x() , t*v.y() , t*v.z() );
}
vector3d operator*(const vector3d &v, double t)
{
  return t*v;
}
vector3d operator/(const vector3d &v, double t)  // not checking for division by zero, will take the IEEE standards infinity or NaN
{
  return (1/t) * v;
}

double dot(const vector3d &v1, const vector3d &v2)
{
  return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}
vector3d cross(const vector3d &v1, const vector3d &v2)
{
  return vector3d( v1.y()*v2.z() - v1.z()*v2.y() , v1.z()*v2.x() - v1.x()*v2.z() , v1.x()*v2.y() - v1.y()*v2.x() );
}
vector3d unitVector(const vector3d& v)
{
  return v / v.length();
}

double tripleProduct(const vector3d &v1, const vector3d &v2, const vector3d &v3)
{
  return dot( cross( v1 , v2 ) , v3 );
}

/*
 * functions to add:
 * 
Vector3 Vector3::proj( const Vector3 & v1 ) const  //projection of this onto v1
{
	double k = dot(v1)/(v1.length()*v1.length());
	return k * v1;
}

*/
