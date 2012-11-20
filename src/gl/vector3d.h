/* Daniel Perry
 * originally written in fall 2001 for cs3505
 *
 */

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
//#include <assert.h>
#include <iostream>

class vector3d  {
public:
    
  vector3d(){ data[0]=0; data[1]=0; data[2]=0; }
  vector3d(double e0, double e1, double e2){ data[0]=e0;data[1]=e1;data[2]=e2; }
  vector3d(const vector3d &v){ data[0]=v.x(); data[1]=v.y(); data[2]=v.z(); }

  vector3d operator = (const vector3d & v){ data[0]=v.x(); data[1]=v.y(); data[2]=v.z(); return (*this);}

  double x() const { return data[0]; }
  double y() const { return data[1]; }
  double z() const { return data[2]; }
 
  void setx( double x ) { data[0]=x; }
  void sety( double y ) { data[1]=y; }
  void setz( double z ) { data[2]=z; }

  const vector3d& operator+() const;
  vector3d operator-() const;
  double operator[](int i) const;
  double& operator[](int i);

  vector3d& operator+=(const vector3d &v2);
  vector3d& operator-=(const vector3d &v2);
  vector3d& operator*=(const double t);
  vector3d& operator/=(const double t); // note: not checking for div by zero .

  vector3d& operator*=(const vector3d &v2){ //NOT dot or cross product, just multiplying components.
    setx( v2.x() * x() );
    sety( v2.y() * y() );
    setz( v2.z() * z() );
    return (*this);
  }

  double length() const;
  double squaredLength() const;

  const vector3d &  MakeUnitVector();

  int indexOfMinComponent() const;
  int indexOfMinAbsComponent() const;
  int indexOfMaxComponent() const;
  int indexOfMaxAbsComponent() const;
  double minComponent() const;
  double maxComponent() const;
  double maxAbsComponent() const;
  double minAbsComponent() const;
    
  //private:
    double data[3];
};



bool operator==(const vector3d &t1, const vector3d &t2);
bool operator!=(const vector3d &t1, const vector3d &t2);

std::istream &operator>>(std::istream &is, vector3d &t);
std::ostream &operator<<(std::ostream &os, const vector3d &t);

vector3d operator+(const vector3d &v1, const vector3d &v2);
vector3d operator-(const vector3d &v1, const vector3d &v2);

vector3d operator*(double t, const vector3d &v);
vector3d operator*(const vector3d &v, double t);
vector3d operator*(const vector3d & v1 , const vector3d & v2 );//added '05 -NOT DOT OR CROSS PRODUCT - MULTIPLICATION OF SCALAR ELEMENTS...
vector3d operator/(const vector3d &v, double t);

vector3d operator/(const vector3d &v1 , const vector3d &v2);

double dot(const vector3d &v1, const vector3d &v2);
vector3d cross(const vector3d &v1, const vector3d &v2);
vector3d unitVector(const vector3d& v);

double tripleProduct(const vector3d &v1, const vector3d &v2, const vector3d &v3);


typedef vector3d Point;
typedef vector3d Vector;

#endif
