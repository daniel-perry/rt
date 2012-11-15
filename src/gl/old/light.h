#ifndef _LIGHT_H
#define _LIGHT_H

#include "rgb.h"
#include "vector3d.h"

class light
{
 public:
  light() 
  {
    setdirection( vector3d(0,-1,0) );
    setcolor( rgb(1,1,1) );
  }
  light( const vector3d & d, const rgb & c )
  {
    setdirection( d );
    setcolor( c );
  }

  vector3d direction() const  { return myDirection; }
  //vector3d & d() { return myDirection; }
  rgb color() const { return myColor; }
  //rgb & c() { return myColor; }

  void setdirection( const vector3d & d ) { myDirection = vector3d(d); }
  void setcolor( const rgb & c) { myColor = rgb(c); }

  //private:
  vector3d myDirection;
  rgb myColor;
};

#endif
