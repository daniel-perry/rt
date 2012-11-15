#include "hbv_node.h"

bool hbv_node :: hit (const ray & r, double tmin, double tmax, double & t, surfacevals & hv ) const
{
  if ( ! box.hit( r, tmin, tmax ) )
    return false;
  
  bool hitL, hitR;
  double tL, tR;
  surfacevals hvL, hvR;

  hitL = left!=0 && left->hit( r, tmin, tmax, tL, hvL );
  hitR = right!=0 && right->hit( r, tmin, tmax, tR, hvR );

  if ( hitL && hitR )
  {
    if ( tL <= tR )
    {
      t = tL;
      hv = hvL;
      return true;
    }
    else
    {
      t = tR;
      hv = hvR;
      return true;
    }
  }
  else if ( hitL )
  {
    t = tL;
    hv = hvL;
    return true;
  }
  else if ( hitR )
  {
    t = tR;
    hv = hvR;
    return true;
  }
  else
  {
    return false;
  }
}








