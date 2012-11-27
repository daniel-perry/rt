/* Daniel Perry
 * for cs6620 - spring 2005
 * started 23mar05
 */


/* BVH : bounding volume hierarchy class
*/

#ifndef _BVH_H
#define _BVH_H

#include "Object.h"
#include "Group.h"

class BVH : public Object
{
 public:
  BVH(){
    left = NULL;
    right = NULL;
  }
  void preprocess(){}
  bool intersect( HitRecord & hit , const RenderContext & context, const ray & r );
  BBox getBBox();

  void buildTree( Group & gr , int axis,int count);

  std::string toString(){return "BVH";}  

  BBox bbox;
  Object * left , * right;
};

#endif
