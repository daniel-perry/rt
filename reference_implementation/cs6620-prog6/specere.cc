
#include "Background.h"
#include "Box.h"
#include "Camera.h"
#include "ConstantBackground.h"
#include "Color.h"
#include "DielectricMaterial.h"
#include "Disk.h"
#include "Filter/Filters.h"
#include "GriddedGroup.h"
#include "Group.h"
#include "Heightfield.h"
#include "Image.h"
#include "LambertianMaterial.h"
#include "Light.h"
#include "Material.h"
#include "MetalMaterial.h"
#include "Object.h"
#include "OrenNayarMaterial.h"
#include "PhongMaterial.h"
#include "PinholeCamera.h"
#include "Plane.h"
#include "PointLight.h"
#include "QAEB.h"
#include "Ray.h"
#include "Ring.h"
#include "Scene.h"
#include "Sphere.h"
#include "Time.h"
#include "Triangle.h"
#include "info.h"
#include <iomanip>
#include <iostream>
//#include "scenes/req2.h"
//#include "scenes/req3.h"
//#include "scenes/req4.h"
//#include "scenes/req5.h"
#include "scenes/req6.h"
//#include "scenes/qaeb_scene.h"
//#include "scenes/highlight_test.h"
//#include "scenes/prog2diel.h"
//#include "scenes/prog2metal.h"
//#include "scenes/prog2orennayar.h"
//#include "scenes/prog2phong.h"
//#include "scenes/attenuation.h"
using std::cerr;
using std::setprecision;
using std::string;

int main(int argc, char** argv)
{
  double t1 = Time::currentSeconds();
  int xres = 512, yres = 512;
  string filename = "image.ppm";

  Image* image = new Image(xres, yres);

  Scene* scene = make_scene();
  scene->setImage(image);
  scene->preprocess();

  double t2 = Time::currentSeconds();
  scene->render();

  double t3 = Time::currentSeconds();
  image->write(filename);
  double t4 = Time::currentSeconds();
  cerr << "------------------------\n";
  cerr << "     Assignment #" << assignment << "\n";
  cerr << "------------------------\n";
  cerr << info;
  cerr << "Setup/load time:\t" << setprecision(3) << t2-t1 << " seconds\n";
  cerr << "Render time:\t\t" << setprecision(3) << t3-t2 << " seconds\n";
  cerr << "Post-process time:\t" << setprecision(3) << t4-t3 << " seconds\n";
  return 0;
}
