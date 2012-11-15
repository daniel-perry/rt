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

class BBox;
//#include "BBox.h"

#include "HitRecord.h"
#include "RenderContext.h"
#include "ray.h"


class Object
{
 public:
  virtual void preprocess()=0;
  virtual bool intersect( HitRecord & hit , const RenderContext & context, const ray & r ) = 0;

  virtual BBox getBBox()=0; 

  virtual char *  print()=0;
  // returns the bounding box of the object.
  
  //private:
};
/*
ostream & operator<<(ostream &os , Object & o ){
  os << o.print();
  return os;
}
ostream & operator<<(ostream &os , Object * o ){
  os << o->print();
  return os;
}
*/
#endif
