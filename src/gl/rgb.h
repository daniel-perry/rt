#include <iostream>
#include <cassert>

#ifndef _RGB_H
#define _RGB_H

using namespace std;

//namespace dperry
//{

class rgb
{
 public:
  rgb()  //default color is black 
  { 
    data[0]=0; 
    data[1]=0; 
    data[2]=0; 
  }
  rgb( float r, float g, float b ) 
  { 
    setr(r); 
    setg(g); 
    setb(b); 
  }
  rgb( const rgb & c1 ) 
  { 
    setr(c1.r()); 
    setg(c1.g()); 
    setb(c1.b()); 
  }

  float r() const 
  { 
    return data[0]; 
  }
  float g() const 
  { 
    return data[1]; 
  }
  float b() const 
  { 
    return data[2]; 
  }
  
  void setr( float r ) 
  { 
    data[0] = r;//inRange(r); 
  }
  void setg( float g ) 
  { 
    data[1] = g;//inRange(g); 
  }
  void setb( float b ) 
  { 
    data[2] = b;//inRange(b); 
  }
  
  float operator [] ( int i ) const;
  float & operator [] ( int i );

  const rgb & operator += ( const rgb & c1 );
  const rgb & operator -= ( const rgb & c1 );
  const rgb & operator *= ( const rgb & rhs );
  const rgb & operator *= ( float k );
  const rgb & operator /= ( float k );
  
 private:
  float data[3];
  float inRange( float c ) const; // makes sure r,g,b are in [0,1]
};

rgb operator + (const rgb& lhs, const rgb& rhs);
rgb operator - (const rgb& lhs, const rgb& rhs);
rgb operator * (const rgb& lhs, const rgb& rhs);
rgb operator * (const rgb& lhs, float k );
rgb operator * ( float k, const rgb& rhs );
rgb operator / (const rgb& lhs, float k );

std::ostream & operator << ( std::ostream & os, const rgb & c1 );
std::istream & operator >> ( std::istream & is, rgb & c1 ); //not protected from entering values outside of [0,1]

bool operator == (const rgb& lhs, const rgb& rhs);
bool operator != (const rgb& lhs, const rgb& rhs);
//}//end namespace dperry

typedef rgb Color;

#endif









