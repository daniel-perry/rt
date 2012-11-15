
#include "Scene.h"
#include "Background.h"
#include "Camera.h"
#include "HitRecord.h"
#include "Filter/Filters.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Object.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Sampler.h"
#include <float.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

Scene::Scene()
{
  object = 0;
  background = 0;
  camera = 0;
  ambient = Color(0, 0, 0);
  image = 0;
  minAttenuation = 0;
  numSamples = 1;
  sampleType = Uniform;
  filter = new Filter2DWrapper<SeparableKernel2D<BoxKernel1D> >;
}

Scene::~Scene()
{
  delete object;
  delete background;
  delete camera;
  delete image;
  for(int i=0;i<static_cast<int>(lights.size());i++){
    Light* light = lights[i];
    delete light;
  }
  delete filter;
}

void Scene::setFilter(Filter2D* new_filter)
{
  if(filter)
    delete filter;
  filter = new_filter;
}

Filter2D* Scene::getFilter() const
{
  return filter;
}

void Scene::preprocess()
{
  background->preprocess();
  for(int i=0;i<static_cast<int>(lights.size());i++){
    Light* light = lights[i];
    light->preprocess();
  }
  double aspect_ratio = image->aspect_ratio();
  camera->preprocess(aspect_ratio);
  object->preprocess();
}

struct ImageSaver {
public:
  ImageSaver(Image* image)
    : image(image)
  {}
  void operator()(int j, int i, const Color& filteredvalue)
  {
    image->set(j, i, filteredvalue);
  }
private:
  Image* image;
};

struct CastRay {
public:
  CastRay(Camera* camera, Scene* scene, const RenderContext& context)
    : camera(camera), scene(scene), context(context)
  {
  }
  ~CastRay()
  {
  }

  Color operator()(float x, float y)
  {
    Ray ray;
    camera->makeRay(ray, context, x, y);
    Color result;
    scene->traceRay(result, context, ray, Color(1,1,1), 0);
    return result;
  }
private:
  Camera* camera;
  Scene* scene;
  const RenderContext& context;
};

void Scene::render()
{
  if(!object || !background || !camera || !image){
    cerr << "Incomplete scene, cannot render!\n";
    exit(1);
  }
  int xres = image->getXresolution();
  int yres = image->getYresolution();
  RenderContext context(this);
  double dx = 2./xres;
  double xmin = -1. + dx/2.;
  double dy = 2./yres;
  double ymin = -1. + dy/2.;
  Color atten(1,1,1);
  int samples = numSamples;
  if(samples == 1 && sampleType == Uniform){
    for(int i=0;i<yres;i++){
      //cerr << "y=" << i << '\n';
      double y = ymin + i*dy;
      for(int j=0;j<xres;j++){
        double x = xmin + j*dx;
        //cerr << "x=" << j << ", y=" << i << '\n';
        Ray ray;
        camera->makeRay(ray, context, x, y);
        HitRecord hit(DBL_MAX);
        object->intersect(hit, context, ray);
        Color result;
        if(hit.getPrimitive()){
          // Ray hit something...
          const Material* matl = hit.getMaterial();
          matl->shade(result, context, ray, hit, atten, 0);
        } else {
          background->getBackgroundColor(result, context, ray);
        }
        image->set(j, i, result);
      }
    }
  } else if(sampleType == Jittered){
    Sampler<Color> sampler(samples, samples, filter,
                           xres, yres);
    sampler.sampleAndFilterRegion(0, xres, 0, yres, drand48, CastRay(camera, this, context), ImageSaver(image));
  } else {
    Sampler<Color> sampler(samples, samples, filter,
                           xres, yres);
    sampler.sampleAndFilterRegion(0, xres, 0, yres, uniform, CastRay(camera, this, context), ImageSaver(image));
  }
}

double Scene::traceRay(Color& result, const RenderContext& context, const Ray& ray, const Color& atten, int depth) const
{
  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
    result = Color(0, 0, 0);
    return 0;
  } else {
    HitRecord hit(DBL_MAX);
    object->intersect(hit, context, ray);
    if(hit.getPrimitive()){
      // Ray hit something...
      const Material* matl = hit.getMaterial();
      matl->shade(result, context, ray, hit, atten, depth);
      return hit.minT();
    } else {
      background->getBackgroundColor(result, context, ray);
      return DBL_MAX;
    }
  }
}

double Scene::traceRay(Color& result, const RenderContext& context, const Object* obj, const Ray& ray, const Color& atten, int depth) const
{
  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
    result = Color(0, 0, 0);
    return 0;
  } else {
    HitRecord hit(DBL_MAX);
    obj->intersect(hit, context, ray);
    if(hit.getPrimitive()){
      // Ray hit something...
      const Material* matl = hit.getMaterial();
      matl->shade(result, context, ray, hit, atten, depth);
      return hit.minT();
    } else {
      background->getBackgroundColor(result, context, ray);
      return DBL_MAX;
    }
  }
}
