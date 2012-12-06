/* Daniel Perry
 * for cs6620 - spring 2005
 * started 27jan05
 */


/* Object: superclass of all things to be intersected
 *
 * direct subclasses: Group
 *                    Primitive (difference? has a normal)
 *                    BoundingBox
 */

#ifndef _OBJECT_H
#define _OBJECT_H

// std
#include <string>

// local
class BBox;
#include "HitRecord.h"
#include "RenderContext.h"
#include "ray.h"

class Object
{
 public:
  virtual void preprocess()=0;
  virtual bool intersect( HitRecord & hit , const RenderContext & context, const ray & r ) const = 0;
  virtual BBox getBBox()=0; 
  virtual std::string toString()=0;
};

ostream & operator<<(ostream &os , Object & o );
ostream & operator<<(ostream &os , Object * o );

#endif
