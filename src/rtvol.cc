/* Daniel Perry
 * Spring 05 - for cs6620
 * ray tracer - started 24 Jan 05
 *
 */

// std
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include <unistd.h>

#include "gl/image.h"
#include "gl/rgb.h"
#include "gl/vector3d.h"

#include "RayTracer.h"

// global vars!
std::string out_fn;
RayTracer * rayTracer;

void saveImage(unsigned char * buffer, int width, int height, int channels, const std::string & fn)
{
	std::cout << "INFO: saving image \"" << fn << "\"." << std::endl;
  image THEIMAGE(width,height);

  for( int i=0; i<width; i++ ){
    for(int j=0; j<height; j++){
			unsigned char * color = &buffer[i*height*channels + j*channels];
      rgb clr( color[0]/255.0, color[1]/255.0, color[2]/255.0 );
      THEIMAGE.set( i , j , clr);
    }
  }
  
  std::ofstream out(fn.c_str());
	if(!out){
    cerr<<"Couldn't open "<<fn<<endl;
    exit(1);
  }
  THEIMAGE.writeppm( out );
}


void doneRendering()
{
  std::cerr << "done rendering." << std::endl;
	const int channels = 4;
	saveImage(rayTracer->getBuffer(), rayTracer->getWidth(), rayTracer->getHeight(), channels, out_fn);
}


int main(int argc, char * argv[] )
{

  if( argc < 2 )
  {
    //std::cerr << "usage: " << argv[0] << " <out_fn> <nrrd_fn> <cmap_fn> <width> <height> <curv-thickness> <norm-flipped>" << std::endl;
    std::cerr << "usage: " << argv[0] << " <out_fn> <points_fn> <values_fn> <cmap_fn> <width> <height> " << std::endl;
    exit(1);
  }

	out_fn = argv[1];
  //std::string nrrd_fn = argv[2];
  //std::string cmap_fn = argv[3];
  std::string points_fn = argv[2];
  std::string values_fn = argv[3];
  std::string cmap_fn = argv[4];
	int width = atoi(argv[5]);
	int height = atoi(argv[6]);
  //float curvThick = atof(argv[6]);
  //bool normFlipped = atoi(argv[7]) != 0;

	//vector3d eye(-.01,-.01,-.01);
	//vector3d eye(-.1,-.1,-.1);
	vector3d eye(-.1,-.1,-.1);
	vector3d lookat(0,0,0);
	vector3d nup(0,0,1);
	double theta = 55;
	double aspectRatio=1;

	rayTracer = new RayTracer(width,height);
	rayTracer->setDoneRenderingCB( & doneRendering );
	//rayTracer->makeScene( nrrd_fn, cmap_fn, curvThick, normFlipped );
	rayTracer->makeScene( points_fn, values_fn, cmap_fn );
  PinholeCamera * ph = dynamic_cast<PinholeCamera*>(rayTracer->getScene()->camera);
  ph->initialize(eye,lookat,nup,theta,aspectRatio);
  rayTracer->restartRender();

	bool done = false;
	while (!done)
	{
		sleep(3);
		done = rayTracer->checkProgress();
	}

  return 0;
}
