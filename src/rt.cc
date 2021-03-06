/* Daniel Perry
 * Spring 05 - for cs6620
 * ray tracer - started 24 Jan 05
 *
 */

// std
#include <cstdlib>
using namespace std;
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "rt.h"

//#include "req.h" 
#include "volume_scene.h"


void printTree( BVH * bvh ){
  cerr<<"bbox:"<<bvh->bbox.bounds[0]<<","<<bvh->bbox.bounds[1]<<endl;
  if(bvh->left!=NULL){
    cerr<<"left:"<<bvh->left;
  }else{
    cerr<<"left:NULL";
  }
  if(bvh->right!=NULL){
    cerr<<" right:"<<bvh->right<<endl;
  }else{
    cerr<<" right:NULL"<<endl;
  }

  if( bvh->left!=NULL && bvh->left->toString() == "BVH" ){
    cerr<<"left node:"<<endl;
    printTree( (BVH*)bvh->left );
  }

  if( bvh->right!=NULL && bvh->right->toString() == "BVH" ){
    cerr<<"right node:"<<endl;
    printTree( (BVH*)bvh->right );
  }
}


int main(int args, char * argv[] )
{

  if( args != 2 ){
    cerr<<"usage: "<<argv[0]<<" outfilename"<<endl;
    exit(1);
  }

  ofstream out(argv[1]);

  if(!out){
    cerr<<"Couldn't open "<<argv[1]<<endl;
    exit(1);
  }

  Time time;
  time.initialize();
  double setupt,render,finalize;


  Scene  * scene =  make_scene() ; // make_scene() should be defined in an #included file above.
  cerr<<"scene loaded."<<endl;
  /* This section is to activate the BVH accel structure....

  BVH * bvh = new BVH();

  Group gr = *((Group*)scene->object);
  cerr<<"gr:"<<(int)gr.objects.size()<<endl;

  bvh->buildTree( gr , 0 , 0);
  scene->object = bvh;
  
  */

  //cerr<<endl<<"Tree makeup:"<<endl;
  //printTree( bvh );
  //cerr<<endl<<endl;
  
  //scene->sampler->setAllSampleCoords();

  int yres=scene->imageY,xres=scene->imageX;  

  image THEIMAGE(xres,yres);

  HitRecord hit;
  RenderContext rc(scene);
  scene->m_rc = rc;
  scene->m_rc.scene = scene;

  //scene->object->intersect( hit , rc , ray( vector3d(0,0,0),vector3d(1,0,0) ) );

  setupt = time.currentSeconds();

  // update on progress:
  //int total = xres * yres,remaining=total;

  cerr<<"ready to render:"<<endl;

  for( int i=0; i<xres; i++ ){
    for(int j=0; j<yres; j++){
      rgb clr;
      scene->sampler->sample( i , j , clr );
      THEIMAGE.set( i , j , clr);
    }
  }
  
  render = time.currentSeconds();
  
  ///// DEBUG
  /*
  cerr<< " weight(0,0):"<<scene->filter->getWeight( 0. , 0. )<<endl;
  cerr<< " weight(1,1):"<<scene->filter->getWeight( 1. , 1. )<<endl;
  cerr<< " weight(.5,.5):"<<scene->filter->getWeight( .25 , .25 )<<endl;
  */
  //// DEBUG


  THEIMAGE.writeppm( out );
  
  finalize = time.currentSeconds();

  finalize -= (setupt+render);
  render -= setupt;

  cerr<<"setup: "<<setupt<<"\nrender: "<<render<<"\nfinalize: "<<finalize<<endl;

  return 0;
}
