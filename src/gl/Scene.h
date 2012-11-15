/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 */

/* Scene: holds all the elements of a scene.
 *
 *
 */


#ifndef _SCENE_H
#define _SCENE_H

using namespace std;

#include <vector>

class Object;
//#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Background.h"
#include "rgb.h"
class Sample;
//#include "Sample.h"
#include "Filter.h"
//class HitRecord;
#include "HitRecord.h"
//class RenderContext;
#include "RenderContext.h"

class Scene
{
 public:
  Scene(){ /*rc.scene = this;*/}
  Scene(Object * obj, Camera * cam, const vector<Light *> & ls, const rgb & AL , Background * bg){
    object = obj;
    camera = cam;
    lights = ls;
    AmbientLight = AL;
    background = bg;
    MaxRayDepth = 0;
    MinAttenuation = 0;

    //rc.scene = this;
  }

  Scene(Object * obj, Camera * cam, const vector<Light *> & ls, const rgb & AL , Background * bg, int mrd, double ma){
    object = obj;
    camera = cam;
    lights = ls;
    AmbientLight = AL;
    background = bg;
    MaxRayDepth = mrd;
    MinAttenuation = ma;

    //rc.scene = this;
  }

  Object * object;
  Camera * camera;
  
  vector<Light *> lights;
  rgb AmbientLight;
  Background * background;
  int MaxRayDepth;
  double MinAttenuation;

  Sample * sampler;
  Filter * filter;

  int imageX, imageY;

  // for render function:
  HitRecord hit;
  RenderContext rc;

  void setBackground( Background * bg){ background = bg; }
  void setObject( Object * obj){ object = obj; }
  void setAmbient( const rgb & AL){ AmbientLight = AL; }
  void addLight( Light * l ){ lights.push_back(l); }
  void setCamera( Camera * cam){ camera = cam; }
  void setMaxRayDepth( int mrd ) { MaxRayDepth=mrd; }
  void setMinAttenuation( double ma ){ MinAttenuation=ma;}
  void setSampler( Sample * s ){ sampler = s;}
  void setFilter( Filter * f ){ filter = f; }

  void render( Color & clr , const ray & r );

 private:

};


#endif
