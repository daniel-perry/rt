#ifndef _SURFACETEXTURE_H
#define _SURFACETEXTURE_H

#include "texture.h"
#include "rgb.h"
#include "vector3d.h"
#include "image.h"
#include <fstream>
#include <cmath>

using namespace std;

class surfacetexture : public texture
{
 public:
  surfacetexture()
    : u0(0),v0(0), u1(0),v1(0), u2(0),v2(0)
  {}

  surfacetexture( const char * filename , int nu0 , int nv0 , int nu1 , int nv1 , int nu2 , int nv2 )
    : u0(nu0),v0(nv0), u1(nu1),v1(nv1), u2(nu2),v2(nv2)
  {
    ifstream sourceImage( filename );
    source.readppm( sourceImage );
  }

  rgb color( const vector3d & beta_gamma )
  {
    double beta = beta_gamma.x(), 
           gamma = beta_gamma.y();
    //cerr<<"surfacetexture.color(..): source.writeppm(cerr):"<<endl;
    //source.writeppm(cerr);
    cerr<<"in surfacetexture.color(..):"<<endl
	<<"calling source.get("<<(int)floor(u0 + beta*u1 + gamma*u2)<<" , "<<(int)floor(v0 + beta*v1 + gamma*v2) <<" )"<<endl;

    return source.get( (int)floor(u0 + beta*u1 + gamma*u2) , (int)floor(v0 + beta*v1 + gamma*v2) );
  }
  
  void setsource( const char * filename )
  {
    ifstream sourceImage( filename );
    source.readppm( sourceImage );
  }

  //private:
  int u0,v0, u1,v1, u2,v2;
  image source;
};

#endif
