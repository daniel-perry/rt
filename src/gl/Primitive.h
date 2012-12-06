/* Daniel Perry
 * for cs6620 - spring 2005
 * started 27jan05
 */


/* Primitive: superclass of all objects with a normal and surface properties.
 *
 * direct subclasses: Sphere, Plane, Cube, Triangle,  Disc
 *
 */


#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

using namespace std;

#include "Object.h"
#include "vector3d.h"
#include "HitRecord.h"
#include "RenderContext.h"
#include "ray.h"
#include "hpoint.h"

class Primitive : public Object
{
 public:
  virtual bool intersect( HitRecord & hit , const RenderContext & context, const ray & r ) const = 0;
  
  // normal returned is guaranteed to be normalized.
  virtual vector3d normal( const vector3d & p ) const = 0;
};


#endif
