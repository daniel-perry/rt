//#include <math.h>
#include "viewerscene.h"
#include "sphere.h"
#include "triangle.h"
#include "axisbox.h"
#include "light.h"
#include "surfacetexture.h"
#include "solidtexture.h"
#include <string>

#include <values.h>  //for MINFLOAT
#define EPSILON MINFLOAT //this needs to be the same as the EPSILON in hpoint.h 
#define DELTA .1//for offsetting the points on a triangle when any of them equal eye.z(), because that causes a divide by zero error.

using namespace std;

//#include <stdlib.h>

//#define MYMAXFLOAT 3.40282347e+38F
// grabbed that number from Linux /usr/include/math.h
// Linux: MAXFLOAT, Sun: FLT_MAX

void viewerscene::read( istream & is)
{
  bool texture_flag=false;
  string objectName;
  sphere s1;
  triangle t1;
  axisbox b1;
  light l1;
  surfacetexture s_texture;
  hpoint gaze(0,0,0,1),vup(0,0,0,1);  // using h-val as a flag to test if one has been read in or not
  dynarray<triangle> tri_array;
  
  aa_levels = 0;

  while( is >> objectName )   
  {
    if ( objectName == "sphere" )
    {
      is >> s1.myCenter >> s1.myRadius >> s1.myMatteColor >> s1.myMirrorColor >> s1.myHighlightConst >> s1.myPhongExponent;
      s1.myMatteColor/=255;
      s1.myMirrorColor/=255;

      surfaceList.append(new sphere( s1 ) );
    }
    else if ( objectName == "triangle" )
    {
      is >> t1.myP0 >> t1.myP1 >> t1.myP2 >> t1.myMatteColor >> t1.myMirrorColor >> t1.myHighlightConst >> t1.myPhongExponent;
      t1.myMatteColor/=255;
      t1.myMirrorColor/=255;
      if (texture_flag)
      {
	t1.myTexture = new surfacetexture(s_texture);
	texture_flag = false;
      }
      triangleList.append( new triangle( t1 ) );
    }
    else if ( objectName == "box" )
    {
      is >> b1.myP0 >> b1.myP1 >> b1.myMatteColor >> b1.myMirrorColor >> b1.myHighlightConst >> b1.myPhongExponent;
      b1.myMatteColor/=255;
      b1.myMirrorColor/=255;
      if (texture_flag)
      {
	b1.myTexture = new surfacetexture( s_texture );
	texture_flag = false;
      }

      surfaceList.append( new axisbox( b1 ) );
      tri_array = b1.triangulate();
      for (int i=0;i<tri_array.length();i++)
	triangleList.append( new triangle(tri_array[i]) );
    }
    else if ( objectName == "light" )
    {
      is >> l1.myDirection >> l1.myColor;
      l1.myColor/=255;
      l1.setdirection( l1.direction()/l1.direction().length() ); // normalize
      addlight( light( l1 ) );
    }
    else if ( objectName == "ambient" )
    {      
      is >> myAmbient;
      myAmbient/=255;
    }
    else if ( objectName == "background" )
    {
      is >> myBackground;
      myBackground/=255;
    }
    else if (objectName == "eye" )
    {
      is >> eye;
      eye.seth(1); //because it's a point
    }
    else if (objectName == "gaze" )
    {
      is >> gaze;
      gaze[3]=0; //because it's a vector ([3]=h)
      //cerr<<"in gaze:"<<gaze<<endl;
    }
    else if (objectName == "Vup" || objectName == "vup")
    {
      is >> vup;
      vup[3]=0; //because it's a vector ([3]=h)
      //cerr<<"in vup:"<<vup<<endl;
    }
    else if (objectName == "viewvolume" || objectName == "ViewVolume")
    {
      is >> l >> b >> n >> r >> t >> f;
    }
    else if (objectName == "nxy" || objectName == "imagesize")
    {
      is >> nx >> ny;
    }
    else if (objectName == "antialiasing" || objectName == "anti-aliasing" || objectName == "aa" )
    {
      is >> aa_levels;
    }
    //texture use in geom file:
    // texture filename.ppm u0 v0 u1 v1 u2 v2
    // object texture is to be mapped to.
    else if (objectName == "texture" || objectName == "surfacetexture" || objectName == "2dtexture")
    {
      is >> objectName;
      s_texture.setsource( objectName.c_str() );
      is >> s_texture.u0 >> s_texture.v0 >> s_texture.u1 >> s_texture.v1 >> s_texture.u2 >> s_texture.v2;
      texture_flag = true;
    }
  }
  
  //cerr<<"g.h:"<<gaze.h()<<" v.h:"<<vup.h()<<endl;
  if ( gaze.h() == 0 && vup.h() == 0 )
  {
    w = -gaze/gaze.distance(); 
    u = cross( vup , w )/cross( vup , w ).distance();
    v = cross( w , u );
    //cerr<<"made it here"<<endl;
  }

  // offset any triangle pts whose z == eye.z by DETA
  // doesn't matter which side I offset to untill I add clipping
  for( int i=0 ; i<triangleList.length() ; i++ )
  {
    if ( (*triangleList[i]).p0().z()-eye.z()<=EPSILON && (*triangleList[i]).p0().z()-eye.z()>=-EPSILON )
    {
      //cerr<<"changed triangle["<<i<<"].p0"<<endl
      //  <<"from:"<<(*triangleList[i]).p0();
      (*triangleList[i]).myP0.setz((*triangleList[i]).p0().z()+DELTA);
      //cerr<<" to:"<<(*triangleList[i]).p0()<<endl;
    }
    if ( (*triangleList[i]).p1().z()-eye.z()<=EPSILON && (*triangleList[i]).p1().z()-eye.z()>=-EPSILON )
    {
      //cerr<<"changed triangle["<<i<<"].p1"<<endl
      //  <<"from:"<<(*triangleList[i]).p0();
      (*triangleList[i]).myP1.setz((*triangleList[i]).p1().z()+DELTA);
      //cerr<<" to:"<<(*triangleList[i]).p0()<<endl;
    }
    if ( (*triangleList[i]).p2().z()-eye.z()<=EPSILON && (*triangleList[i]).p2().z()-eye.z()>=-EPSILON )
    {
      //cerr<<"changed triangle["<<i<<"].p2"<<endl
      //  <<"from:"<<(*triangleList[i]).p0();
      (*triangleList[i]).myP2.setz((*triangleList[i]).p2().z()+DELTA);
      //cerr<<" to:"<<(*triangleList[i]).p0()<<endl;
    }
      
  }

  nx += aa_levels;
  ny += aa_levels;
}
