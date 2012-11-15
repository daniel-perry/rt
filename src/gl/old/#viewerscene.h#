#ifndef _VIEWERSCENE_H
#define _VIEWERSCENE_H

#include "dynarray.h"
#include "surface.h"
#include "light.h"
#include "triangle.h"
#include "hpoint.h"
//#include "boundingbox.h"
//#include "hbv_node.h"

//debug
#include <fstream>

class viewerscene
{
 public:
  viewerscene () { l=-1;b=-1;n=-1;r=1;t=1;f=1; nx=100;ny=100; eye=hpoint(0,0,0,1); u=hpoint(1,0,0,0);v=hpoint(0,1,0,0);w=hpoint(0,0,1,0); }
  void read(istream & is);
  rgb color( const ray & r, double tmin, double tmax );

  void addlight( const light & l ) { myLights.append(l); }
  void addsurface( surface * s ) { surfaceList.append(s); }
  void setbackground( const rgb & bg ) { myBackground=rgb(bg); }
  void setambient( const rgb & amb ) { myAmbient=rgb(amb); }

  void addtriangle( triangle * t ) { triangleList.append(t); }

  //bool hit( const ray & r, double tmin, double tmax, double & t, surfacevals & sv);

  rgb background() const { return myBackground; }
  rgb ambient() const { return myAmbient; }

  //private:
  dynarray<triangle*> triangleList;
  dynarray<surface*> surfaceList; 
  dynarray<light> myLights;
  rgb myBackground;
  rgb myAmbient;
  hpoint eye,u,v,w;  //arbitrary viewing axis
  double l,b,n, r,t,f; //view volume
  int nx, ny, aa_levels; //output image size
};
    
#endif
