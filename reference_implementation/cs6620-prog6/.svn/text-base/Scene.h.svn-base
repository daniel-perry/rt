
#ifndef Scene_h
#define Scene_h

#include "Color.h"
#include <string>
#include <vector>

class Background;
class Camera;
class Color;
class Filter2D;
class Image;
class Light;
class Object;
class RenderContext;
class Ray;

class Scene {
 public:
  Scene();
  virtual ~Scene();

  inline Object* getObject() const {
    return object;
  }
  void setObject(Object* obj) {
    object = obj;
  }

  inline Background* getBackground() const {
    return background;
  }
  void setBackground(Background* bg) {
    background = bg;
  }

  inline Camera* getCamera() const {
    return camera;
  }
  void setCamera(Camera* cam) {
    camera = cam;
  }

  void setImage(Image* im) {
    image = im;
  }

  void addLight(Light* light) {
    lights.push_back(light);
  }
  const std::vector<Light*>& getLights() const {
    return lights;
  }

  Color getAmbient() const {
    return ambient;
  }
  void setAmbient(const Color& amb) {
    ambient = amb;
  }

  int getMaxRayDepth() const {
    return maxRayDepth;
  }
  void setMaxRayDepth(int rd) {
    maxRayDepth = rd;
  }
  double getMinAttenuation() const {
    return minAttenuation;
  }
  void setMinAttenuation(double atten) {
    minAttenuation = atten;
  }

  enum SampleType {
    Uniform,
    Jittered
  };
  int getNumSamples() const {
    return numSamples;
  }
  void setNumSamples(int samples) {
    numSamples = samples;
  }
  SampleType getSampleType() const {
    return sampleType;
  }
  void setSampleType(SampleType newSampleType) {
    sampleType = newSampleType;
  }
  void setSampling(SampleType newSampleType, int samples) {
    sampleType = newSampleType;
    numSamples = samples;
  }
  Filter2D* getFilter() const;
  void setFilter(Filter2D* filter);
  

  void preprocess();
  void render();
  double traceRay(Color& result, const RenderContext& context, const Ray& ray, const Color& attenuation, int depth) const;
  double traceRay(Color& result, const RenderContext& context, const Object* obj, const Ray& ray, const Color& attenuation, int depth) const;

 private:
  Scene(const Scene&);
  Scene& operator=(const Scene&);

  Background* background;
  Camera* camera;
  Color ambient;
  Image* image;
  Object* object;
  Filter2D* filter;
  std::vector<Light*> lights;
  int maxRayDepth;
  double minAttenuation;
  int numSamples;
  SampleType sampleType;
};

#endif
