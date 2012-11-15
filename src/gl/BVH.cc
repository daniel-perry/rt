/* Daniel Perry
 * for cs6620 - spring 2005
 * started 23mar05
 */


/* BVH : bounding volume hierarchy class
*/
#include <iostream>
using namespace std;


#include "BVH.h"

bool BVH::intersect( HitRecord & hit , const RenderContext & context, const ray & r ){
  /* cerr<<"\n\nbvh intersect:"<<endl;
  cerr<<"bbox:"<<bbox.bounds[0]<<","<<bbox.bounds[1]<<endl;
  cerr<<"left:"<<left->print()<<" right:"<<right->print()<<endl;
  */
  hit.bbox_t = MYMAX;
  if( bbox.intersect( hit , context , r ) ){
    if( left!=NULL && left->intersect( hit , context , r ) ){
      return true;
    }
    else{
      return (right!=NULL && right->intersect( hit , context , r ));
    }
  }
  return false;
}

BBox BVH::getBBox(){
  return bbox;
}


void BVH::buildTree( Group & gr , int axis, int count){

  //cerr<<"\n\nmade it here:"<<count<<endl;
  //cerr<<"gr.size:"<<gr.objects.size()<<endl;

  if( gr.objects.size() == 1 ){
    //cerr<<"entered size 1"<<endl;
    left = gr.objects[0];
    right = NULL;
    bbox = left->getBBox();
    return;
    //return this;
  }
  if( gr.objects.size() == 2 ){
    //cerr<<"entered size 2"<<endl;
    left = gr.objects[0];
    right = gr.objects[1];
    
    bbox = merge( left->getBBox() , right->getBBox() );

    return;
    //return this;
  }

  //cerr<<"entering main body"<<endl;

  bbox = gr.getBBox();
  Point centroid = bbox.centroid(),temp_centroid;
  //cerr<<"dims:"<<bbox.bounds[0]<<", "<<bbox.bounds[1]<<endl;
  //cerr<<"centroid:"<<centroid<<endl;

  // may need to allocate on heap and pass by pointer...
  Group left_gr , right_gr;

  int deb_r = 0, deb_l = 0;

  // sort according to axis:
  for( int i=0; i<(int)gr.objects.size() ; i++){
    BBox temp_bbox = gr.objects[i]->getBBox();
    temp_centroid = gr.objects[i]->getBBox().centroid();
    /*if( count > 2000 ){
      cerr<<"temp dims:"<<temp_bbox.bounds[0]<<","<<temp_bbox.bounds[1]<<endl;
      cerr<<"temp centroid:"<<temp_centroid<<endl;
      }*/
    if( temp_centroid[axis] <= centroid[axis] ){
      left_gr.addObject( gr.objects[i] );
      deb_l++;
    }else{
      right_gr.addObject( gr.objects[i] );
      deb_r++;
    }
  }

  //when sorting fails on small cases, just split them up.
  if( ((int)gr.objects.size())<4 && (((int)left_gr.objects.size()) == 0 || ((int)right_gr.objects.size()) == 0 )){
    int temp = ((int)gr.objects.size())/2;
    
    //cerr<<"\n\nbad sort"<<endl;
    //cerr<<"temp:"<<temp<<endl;

    left_gr.objects.clear();
    right_gr.objects.clear();
    for( int i=0; i<temp ; i++){
      left_gr.addObject( gr.objects[i] );
    }
    for( int i=temp; i< (int)gr.objects.size() ; i++){
      right_gr.addObject( gr.objects[i] );
    }
  }

  
  //cerr<<"left num:"<<deb_l<<" right num:"<<deb_r<<endl;

  if( left_gr.objects.size() > 0 ){
    left = new BVH();
    ((BVH*)left)->buildTree( left_gr , (axis+1)%3 , count+1);
  }
  else{
    left = NULL;
  }

  if(right_gr.objects.size() > 0 ){
    right = new BVH();
    ((BVH*)right)->buildTree( right_gr , (axis+1)%3 , count+1);
  }
  else{
    right = NULL;
  }

}
