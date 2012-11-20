#include <iostream>
#include "rgb.h"
//#include "triangle.h"

#ifndef _IMAGE_H
#define _IMAGE_H
//using namespace dperry;

class image
{
 public:
  image();
  image( int, int );
  ~image();
  
  const image & operator = ( const image & im );

  int width() const;
  int height() const;
  
  /* instead of [r][c], use get(x,y) and set(x,y,rgb)
  rgb* operator [] (int r) const;
  rgb* operator [] (int r);
  */
  
  void resetraster( int w , int h );

  const rgb & get( int x, int y) const;
  void set( int x, int y, rgb c);

  void drawline( int x0, int y0, rgb c0, int x1, int y1, rgb c1 );
  void drawtriangle( float x0, float y0, rgb c0, float x1, float y1, rgb c1, float x2, float y2, rgb c2 );
  //void drawtriangle( const triangle & t);
  void readppm ( istream & s );
  void writeppm( ostream & s );
  void writeppmBit( ostream & s );
  
  //for zbuffer
  void set( int x, int y, rgb c, float zb_val );
  void drawtriangle( float x0, float y0, float zbv0, rgb c0, float x1, float y1, float zbv1, rgb c1, float x2, float y2, float zbv2, rgb c2 );  
  
  void antialias();

 private:
  rgb * * raster;
  float **zbuffer;
  int imageWidth,imageHeight;
  unsigned int toInt( float f ) const;
};

inline float f_ab( float xa, float ya, float xb, float yb, float x, float y )
{
  return ( ya - yb ) * x + ( xb - xa ) * y + xa * yb - xb * ya;
}

// helper functions for drawtriangle().
inline float maxNum ( float n1, float n2 )
{
  if (n1 >= n2)
    return n1;
  return n2;
}
inline float minNum ( float n1, float n2 )
{
  if (n1 <= n2)
    return n1;
  return n2;
}

#endif
