#ifndef _SOLIDTEXTURE_H
#define _SOLIDTEXTURE_H

#include "texture.h"
#include "rgb.h"
#include "vector3d.h"

class solidtexture : public texture
{
 public:
  solidtexture();

  rgb color ( const vector3d & realWorld )
  {
    return rgb(0,0,1);
  }

 private:
};

#endif
