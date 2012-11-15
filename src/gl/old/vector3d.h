#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>
#include <assert.h>
#include <iostream.h>

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

  double length() const;
  double squaredLength() const;

  void MakeUnitVector();

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

istream &operator>>(istream &is, vector3d &t);
ostream &operator<<(ostream &os, const vector3d &t);

vector3d operator+(const vector3d &v1, const vector3d &v2);
vector3d operator-(const vector3d &v1, const vector3d &v2);

vector3d operator*(double t, const vector3d &v);
vector3d operator*(const vector3d &v, double t);
vector3d operator/(const vector3d &v, double t);

double dot(const vector3d &v1, const vector3d &v2);
vector3d cross(const vector3d &v1, const vector3d &v2);
vector3d unitVector(const vector3d& v);

double tripleProduct(const vector3d &v1, const vector3d &v2, const vector3d &v3);
#endif
