#include "rgb.h"
/*
rgb::rgb()  //default color is black 
{ 
  data[0]=0; 
  data[1]=0; 
  data[2]=0; 
}
rgb::rgb( float r, float g, float b ) 
{ 
  setr(r); 
  setg(g); 
  setb(b); 
}
rgb::rgb( const rgb & c1 ) 
{ 
  setr(c1.r()); 
  setg(c1.g()); 
  setb(c1.b()); 
}

float rgb::r() const 
{ 
  return data[0]; 
}
float rgb::g() const 
{ 
  return data[1]; 
}
float rgb::b() const 
{ 
  return data[2]; 
}
  
void rgb::setr( float r ) 
{ 
  data[0] = r;//inRange(r); 
}
void rgb::setg( float g ) 
{ 
  data[1] = g;//inRange(g); 
}
void rgb::setb( float b ) 
{ 
  data[2] = b;//inRange(b); 
}
*/

float rgb::operator [] ( int i ) const 
{ 
  assert( i >= 0 && i<3 ); 
  return data[i]; 
}
float & rgb::operator [] ( int i ) 
{ 
  assert( i >= 0 && i<3 ); 
  return data[i]; 
}

const rgb & rgb::operator += ( const rgb & c1 ) 
{ 
  setr(r()+c1.r()); 
  setg(g()+c1.g()); 
  setb(b()+c1.b()); 
  return (*this); 
}
const rgb & rgb::operator -= ( const rgb & c1 ) 
{ 
  setr(r()-c1.r()); 
  setg(g()-c1.g()); 
  setb(b()-c1.b()); 
  return (*this); 
}
const rgb & rgb::operator *= ( const rgb & rhs )
{
  setr(r()*rhs.r());
  setg(g()*rhs.g());
  setb(b()*rhs.b());
  return (*this);
}
const rgb & rgb::operator *= ( float k ) 
{ 
  setr(r()*k); 
  setg(g()*k); 
  setb(b()*k); 
  return (*this); 
}
const rgb & rgb::operator /= ( float k ) 
{ 
  setr(r()/k); 
  setg(g()/k); 
  setb(b()/k); 
  return (*this); 
}
  
float rgb::inRange( float c ) const // makes sure r,g,b are in [0,1]
{ 
  if(c>1.0) 
    return 1.0; 
  if(c<0.0) 
    return 0.0; 
  return c; 
} 

rgb operator + (const rgb& lhs, const rgb& rhs) 
{ 
  rgb c1(lhs); 
  c1+=rhs; 
  return c1; 
}
rgb operator - (const rgb& lhs, const rgb& rhs) 
{ 
  rgb c1(lhs); 
  c1-=rhs; 
  return c1; 
}
rgb operator * (const rgb& lhs, const rgb& rhs)
{
  rgb c1(lhs);
  c1*=rhs;
  return c1;
}
rgb operator * (const rgb& lhs, float k ) 
{ 
  rgb c1(lhs); 
  c1*=k; 
  return c1; 
}
rgb operator * ( float k, const rgb& rhs ) 
{ 
  return rhs * k; 
}
rgb operator / (const rgb& lhs, float k ) 
{ 
  return lhs * (1/k); 
}

ostream & operator << ( ostream & os, const rgb & c1 ) 
{ 
  os<<'('<<c1.r()<<','<<c1.g()<<','<<c1.b()<<')'; 
  return os; 
}
istream & operator >> ( istream & is, rgb & c1 )  //not protected from entering values outside of [0,1]
{ 
  is>>c1[0]>>c1[1]>>c1[2]; 
  return is; 
}

bool operator == (const rgb& lhs, const rgb& rhs)
{
  return lhs.r()==rhs.r() && lhs.g()==rhs.g() && lhs.b()==rhs.b();
}

bool operator != (const rgb& lhs, const rgb& rhs)
{
  return !(lhs==rhs);
}
