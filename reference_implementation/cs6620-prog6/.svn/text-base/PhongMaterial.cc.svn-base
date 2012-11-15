
#include "PhongMaterial.h"
#include "HitRecord.h"
#include "Light.h"
#include "Math.h"
#include "Point.h"
#include "Primitive.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Vector.h"
#include <iostream>
using namespace std;

PhongMaterial::PhongMaterial(const Color& color, float Kd, float Ka, const Color& phong_color,
                             int phong_exponent)
  : color(color), Kd(Kd), Ka(Ka), phong_color(phong_color), phong_exponent(phong_exponent)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::shade(Color& result, const RenderContext& context, const Ray& ray,
                          const HitRecord& hit, const Color&, int) const
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
  //cerr << "normal=" << normal << ", costheta=" << costheta << '\n';

  const Object* world = scene->getObject();
  Light*const* begin = &lights[0];
  Light*const* end = &lights[lights.size()];
  Color difflight = scene->getAmbient()*Ka;
  Color speclight(0, 0, 0);
  while(begin != end){
    Color light_color;
    Vector light_direction;
    double dist = (*begin++)->getLight(light_color, light_direction, context, hitpos);
    double cosphi = Dot(normal, light_direction);
    //cerr << "cosphi=" << cosphi << '\n';
    if(cosphi > 0){
      // Cast shadow rays...
      HitRecord shadowhit(dist);
      Ray shadowray(hitpos, light_direction);
      world->intersect(shadowhit, context, shadowray);
      //cerr << "shadow=" << shadowhit.getPrimitive() << '\n';
      if(!shadowhit.getPrimitive()){
        // No shadows...
        difflight += light_color*(Kd*cosphi);
        Vector H = light_direction - ray.direction();
        H.normalize();
        double cos_alpha = Dot(H, normal);
        if(cos_alpha > 0){
          double phong_term = ipow(cos_alpha, phong_exponent);
          speclight += light_color*phong_term;
        }
      }
    }
  }
  result = difflight * color + speclight * phong_color;
  //cerr << "result=" << result << '\n';
}
