/* Daniel Perry
 * for cs6620 - spring 2005
 * started 27jan05
 */

/* Group: class to hold groups of objects and subclasses.
 *
 *
 *
 */

#ifndef _GROUP_H
#define _GROUP_H

using namespace std;

#include "Object.h"
#include <vector>
#include "BBox.h"

class Group : public Object
{
 public:
  Group(){}
  
  void preprocess(){}
  
  std::string toString(){return "Group";}

  bool intersect( HitRecord & hit , const RenderContext & context, const ray & r ){
    bool hitp = false;
    for( int i=0; i<(int)objects.size(); i++){
      if(objects[i]->intersect( hit , context, r )){
	hitp = true;
      }
    }
    return hitp;
  }
  
  void add( Object * ob ){
    objects.push_back( ob );
  }
  void addObject( Object * ob ){
    objects.push_back( ob );
  }
  
  BBox getBBox(){
    BBox temp, bbox = objects[0]->getBBox();
    for( int i=1; i<(int)objects.size() ; i++){
      temp = objects[i]->getBBox();
      bbox.bounds[0].setx( fmin( temp.bounds[0].x() , bbox.bounds[0].x()) );
      bbox.bounds[0].sety( fmin( temp.bounds[0].y() , bbox.bounds[0].y()) );
      bbox.bounds[0].setz( fmin( temp.bounds[0].z() , bbox.bounds[0].z()) );

      bbox.bounds[1].setx( fmax( temp.bounds[1].x() , bbox.bounds[1].x()) );
      bbox.bounds[1].sety( fmax( temp.bounds[1].y() , bbox.bounds[1].y()) );
      bbox.bounds[1].setz( fmax( temp.bounds[1].z() , bbox.bounds[1].z()) );
    }
    return bbox;
  }
  
  //private:
  vector<Object*> objects;
  
};

#endif
