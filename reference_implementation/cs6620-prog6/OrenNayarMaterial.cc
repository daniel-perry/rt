
#include "OrenNayarMaterial.h"
#include "HitRecord.h"
#include "Light.h"
#include "Math.h"
#include "Point.h"
#include "Primitive.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Vector.h"
#include "Math.h"
#include <iostream>
using namespace std;

OrenNayarMaterial::OrenNayarMaterial(const Color& color, double sigma)
  :color(color)
{
  double s2 = sigma*sigma;
  A = 1-s2/(2*(s2+0.33));
  B = 0.45*s2/(s2+0.09);
}

OrenNayarMaterial::~OrenNayarMaterial()
{
}

void OrenNayarMaterial::shade(Color& result, const RenderContext& context,
                              const Ray& ray, const HitRecord& hit,
                              const Color& atten, int depth) const
{
  const Scene* scene = context.getScene();
  const vector<Light*>& lights = scene->getLights();
  Point hitpos = ray.origin()+ray.direction()*hit.minT();
  Vector normal;
  hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
  double costheta = Dot(normal, ray.direction());
  if(costheta > 0)
    normal = -normal;
  else
    costheta = -costheta;

  const Object* world = scene->getObject();

  double Ka = 0.4;
  Color light = scene->getAmbient()*Ka;

#if 0
  for(vector<Light*>::const_iterator iter = lights.begin(); iter != lights.end(); iter++){
#else
    Light*const* begin = &lights[0];
    Light*const* end = &lights[lights.size()];
    while(begin != end){
#endif
    Color light_color;
    Vector light_direction;
    double dist = (*begin++)->getLight(light_color, light_direction, context, hitpos);
    double cosphi = Dot(normal, light_direction);
    if(cosphi > 0){
      // Cast shadow rays...
      HitRecord shadowhit(dist);
      Ray shadowray(hitpos, light_direction);
      world->intersect(shadowhit, context, shadowray);
      if(!shadowhit.getPrimitive()){
        // No shadows...
        double theta = acos(costheta);
        double phi = acos(cosphi);
        double alpha = Max(theta, phi);
        double beta = Min(theta, phi);
        double fr = A+B*Max(0.0, cos(theta-phi))*sin(alpha)*tan(beta);
        //fr = Max(0, fr);
        //fr = Min(1, fr);
        //cerr << "costheta=" << costheta << ", cosphi=" << cosphi << '\n';
        //cerr << "theta=" << theta << ", phi=" << phi << '\n';
        //cerr << "fr=" << fr*cosphi << ", cos=" << cos(theta-phi) << ", sin=" << sin(alpha) << ", tan=" << tan(beta) << ", A=" << A << ", B=" << B << '\n';
        double Kd = 0.6;
        light += light_color*cosphi*fr*Kd;
      }
    }
  }
  result = light*color;
}
