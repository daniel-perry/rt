/* Daniel Perry
 * for cs6620 - spring 2005
 * started 27jan05
 */

/* RenderContext: to hold the current context of the render.
 * 
 */


#ifndef _RENDERCONTEXT_H
#define _RENDERCONTEXT_H

using namespace std;

class Scene;
//#include "Scene.h"

class RenderContext
{
 public:
  RenderContext(){}
  RenderContext(Scene * sc){ scene = sc;}

  const Scene & getScene() const{ return (*scene);}

  // may want to make this into a point later on (going to affect a lot of references to it).
  Scene * scene;

  //private:

};

#endif
