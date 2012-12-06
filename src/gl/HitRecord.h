/* Daniel Perry
 * for cs6620 - spring 2005
 * started 27jan05
 */

/* HitRecord: keeping track of what objects have hit.
 *
 *
 *
 */

#ifndef _HITRECORD_H
#define _HITRECORD_H

using namespace std;

class Primitive;
//#include "Primitive.h"
#include "Material.h"
#include "util.h"
#include <assert.h>


int const MAXSIZE = 100;

class HitRecord
{
 public:
  HitRecord(){t =MYMAX;bbox_t=MYMAX;prim=0;material=0;data_good=false;}

  bool hit(double new_t, const Vector & n, const Primitive * hit_prim, const Material * hit_matl ){
    if(new_t<t && new_t > MYMIN){
      t = new_t;
      prim = hit_prim;
      material = hit_matl;
      normal = n; 
      return true;
    }
    return false;
  }

  //same as above but doesn't save the hit t (because not going to render bbox).
  bool hit_bbox(double new_t, Primitive * hit_prim, Material * hit_matl ){
    /* if( new_t < bbox_t ){
      cerr<<"hit: new_t:"<<new_t<<" bbox_t:"<<bbox_t<<endl;
    }
    else{
      cerr<<"no hit"<<endl;
      }*/
    if(new_t<t && new_t > MYMIN && new_t < bbox_t){
      bbox_t = new_t;
      //prim = hit_prim;
      //material = hit_matl;
      return true;
    }
    return false;
  }

  Point hitPoint( double t , const ray & r ){
    if( ! data_good )
      return r.eval(t);
    data_good = false;
    return data_point;
  }

  
  void setPoint( const Point & p ){
    data_point = p;
    data_good = true;
  }

  template <typename T> 
    T* getScratchpad(){
    assert( sizeof(char) <= MAXSIZE );
    data_good = true;
    return reinterpret_cast<T*>(data);
  }


  int id;
  bool data_good;
  Point data_point;
  char data[MAXSIZE];
  double t;
  vector3d normal;
  
  double bbox_t;

  const Primitive * prim;
  const Material * material;  

};


#endif

