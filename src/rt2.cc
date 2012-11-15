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
#include "gl/Sample.h"
#include "gl/JitteredSample.h"

#include "gl/Time.h"
#include "gl/util.h"

using namespace std;
#include <fstream>

#include "req.h" // this file defines the scene by defining the function make_scene().

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


  Scene scene = (* make_scene() ); // make_scene() should be defined in an #included file above.

  scene.sampler->setAllSampleCoords();

  int yres=scene.imageY,xres=scene.imageX;  

  image THEIMAGE(xres,yres);

  HitRecord hit;
  RenderContext rc(scene);


  setupt = time.currentSeconds();

  for( int i=0; i<xres; i++ ){
    for(int j=0; j<yres; j++){
      double x = 2 * (i-xres/2. + .5 ) / xres;
      double y = 2 * (j-yres/2. + .5 ) / yres;
      ray r;
      rgb clr,sampled_clr(0,0,0);
      double x_sample, y_sample;
      for( int k=0; k<scene.sampler->numSamples; k++){
	x_sample = x + scene.sampler->samples[i][j][k].x();
	y_sample = y + scene.sampler->samples[i][j][k].y();
	scene.camera->makeRay( r , rc , x_sample , y_sample );
	hit.t = MYMAX;
	if(scene.object->intersect( hit , rc , r )){
	  hit.material->shade( clr , rc , r , hit , 0 , 1);
	}else{
	  scene.background->getBackgroundColor( clr , rc , r );
	}
	sampled_clr += (clr * scene.filter->getWeight( scene.sampler->samples[i][j][k].x(),scene.sampler->samples[i][j][k].y() ));

	//cerr<<"sampled color:"<<clr<<endl<<"filter weight:"<<scene.filter->getWeight( scene.sampler->samples[i][j][k].x(),scene.sampler->samples[i][j][k].y() )<<endl;

      }
      sampled_clr /= scene.sampler->numSamples;
      //cerr<<"final color:"<<sampled_clr<<endl;
      THEIMAGE.set( i , j , sampled_clr );
    }
  }
  
  render = time.currentSeconds();

  THEIMAGE.writeppm( out );
  
  finalize = time.currentSeconds();

  finalize -= (setupt+render);
  render -= setupt;

  cerr<<"setup: "<<setupt<<"\nrender: "<<render<<"\nfinalize: "<<finalize<<endl;

  return 0;
}
