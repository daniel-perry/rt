#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "rgb.h"
#include "vector3d.h"

//debug
//#include "image.h"

class texture
{
 public:
  virtual rgb color( const vector3d & ) = 0;
  //virtual ~texture(){}
  //image source;
 private:
};

#endif
