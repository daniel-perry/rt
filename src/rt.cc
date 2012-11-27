/* Daniel Perry
 * Spring 05 - for cs6620
 * ray tracer - started 24 Jan 05
 *
 */

#include <iostream>
#include <vector>

#include "gl/hpoint.h"
#include "gl/image.h"
#include "gl/ray.h"
#include "gl/rgb.h"
#include "gl/vector3d.h"

#include "gl/Scene.h"
#include "gl/Sphere.h"
#include "gl/Triangle.h"
#include "gl/Disk.h"
#include "gl/Ring.h"
#include "gl/AxisBox.h"
#include "gl/HeightField.h"

#include "gl/LambertianMaterial.h"
#include "gl/PhongMaterial.h"
#include "gl/MetalMaterial.h"
#include "gl/DielectricMaterial.h"
#include "gl/CoupledMaterial.h"
#include "gl/PhongVolumeMaterial.h"

#include "gl/Colormap.h"

#include "gl/Camera.h"
#include "gl/Light.h"
#include "gl/DirectionalLight.h"
#include "gl/PointLight.h"
#include "gl/ConstantBackground.h"
#include "gl/PinholeCamera.h"
#include "gl/Background.h"

#include "gl/Object.h"
#include "gl/Material.h"
#include "gl/Group.h"
#include "gl/Plane.h"

#include "gl/Filter.h"
#include "gl/BoxFilter.h"
#include "gl/TentFilter.h"
#include "gl/GaussianFilter.h"
//#include "gl/LanczosSincFilter.h"

#include "gl/Sample.h"
#include "gl/JitteredSample.h"
#include "gl/UniformSample.h"

//#include "gl/FunctionEval.h"

#include "gl/Time.h"
#include "gl/util.h"

#include "gl/BBox.h"
#include "gl/BVH.h"

#include "gl/matrix.h"
#include "gl/TInstance.h"

#include <cstdlib>
using namespace std;
#include <fstream>

//#include "req.h" // this file defines the scene by defining the function make_scene().
#include "req8.h"

#include <string.h>
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
  scene->rc = rc;
  scene->rc.scene = scene;

  //scene->object->intersect( hit , rc , ray( vector3d(0,0,0),vector3d(1,0,0) ) );

  setupt = time.currentSeconds();

  // update on progress:
  //int total = xres * yres,remaining=total;

  cerr<<"ready to render:"<<endl;

  for( int i=0; i<xres; i++ ){
    for(int j=0; j<yres; j++){
      ray r;
      rgb clr,sampled_clr(0,0,0);
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
