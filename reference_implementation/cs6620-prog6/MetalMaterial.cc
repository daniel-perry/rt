
#include "MetalMaterial.h"
#include "HitRecord.h"
#include "Light.h"
#include "Math.h"
#include "Point.h"
#include "Primitive.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Vector.h"
using namespace std;

MetalMaterial::MetalMaterial(const Color& refl, int phong_exponent)
  : refl(refl), phong_exponent(phong_exponent)
{
  not_refl = Color(1,1,1)-refl;
}

MetalMaterial::~MetalMaterial()
{
}

void MetalMaterial::shade(Color& result, const RenderContext& context, const Ray& ray,
                          const HitRecord& hit, const Color& atten, int depth) const
{
  const Scene* scene = context.getScene();
  const vector<Light*>& lights = scene->getLights();
  Point hitpos = ray.origin()+ray.direction()*hit.minT();
  Vector normal;
  hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
  double costheta = Dot(normal, ray.direction());
  if(costheta > 0){
    normal = -normal;
  } else {
    costheta = -costheta;
  }

  double k = 1-costheta;
  double k2 = k*k;
  double k4 = k2*k2;
  double k5 = k4*k;
  Color R = (refl + not_refl*k5);
  Vector refl_dir = ray.direction()+2*costheta*normal;
  Color refl_color;
  Ray refl_ray(hitpos, refl_dir);
  scene->traceRay(refl_color, context, refl_ray, atten*R, depth+1);
  result = R*refl_color;

  const Object* world = scene->getObject();
  Light*const* begin = &lights[0];
  Light*const* end = &lights[lights.size()];
  Color light(0, 0, 0);
  while(begin != end){
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
        Vector H = light_direction - ray.direction();
        H.normalize();
        double cos_alpha = Dot(H, normal);
        if(cos_alpha > 0){
          double phong_term = ipow(cos_alpha, phong_exponent);
          light += light_color*phong_term;
        }
      }
    }
  }
  result += light * refl;
}
