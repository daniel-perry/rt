#ifndef _SURFACE_H
#define _SURFACE_H

#include "rgb.h"
#include "vector3d.h"
#include "ray.h"
#include "dynarray.h"
#include "texture.h"


struct surfacevals
{
  rgb matte;
  rgb mirror;
  double highlight;
  double phongexponent;
  vector3d normal;
};

class surface
{
 public:
  //~surface()
  //{
  //  delete myTexture;
  //}

  virtual bool hit (const ray & r, double tmin, double tmax, double & t, surfacevals & hv ) const = 0;

  virtual vector3d centroid() const = 0;
  virtual dynarray<vector3d> elements() const = 0;

  rgb matte() const
  {
    return myMatteColor;
  }
  rgb mirror() const
  {
    return myMirrorColor;
  }
  double highlight() const
  {
    return myHighlightConst;
  }
  double phongexponent() const
  {
    return myPhongExponent;
  }

  void setmatte( const rgb & kd )
  {
    myMatteColor = rgb( kd );
  }
  void setmirror( const rgb & ks )
  {
    myMirrorColor = rgb( ks );
  }
  void sethighlight( double kh )
  {
    myHighlightConst = kh;
  }
  void setphongexponent( double e )
  {
    myPhongExponent = e;
  }

  //protected:
  rgb myMatteColor;
  rgb myMirrorColor;
  double myHighlightConst;
  double myPhongExponent;
  texture * myTexture;
};

#endif


