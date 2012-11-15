
#include "DielectricMaterial.h"
#include "HitRecord.h"
#include "Light.h"
#include "Point.h"
#include "Primitive.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Vector.h"
#include "Math.h"
using namespace std;

DielectricMaterial::DielectricMaterial(double eta, int phong_exponent)
  : eta(eta), extinction(1,1,1), phong_exponent(phong_exponent)
{
  extinction_constant = Color(0,0,0);
  inv_eta = 1./eta;
  r0 = (eta-1)/(eta+1);
  r0 = r0*r0;
  nothing_inside = true;
}

DielectricMaterial::DielectricMaterial(double eta, const Color& extinction, int phong_exponent)
  : eta(eta), extinction(extinction), phong_exponent(phong_exponent)
{
  extinction_constant = Color(log(extinction.r()), log(extinction.g()), log(extinction.b()));
  inv_eta = 1./eta;
  r0 = (eta-1)/(eta+1);
  r0 = r0*r0;
  nothing_inside = true;
}

DielectricMaterial::~DielectricMaterial()
{
}

void DielectricMaterial::shade(Color& result, const RenderContext& context,
                               const Ray& ray, const HitRecord& hit, const Color& atten,
                               int depth) const
{
  const Scene* scene = context.getScene();
  const vector<Light*>& lights = scene->getLights();
  Point hitpos = ray.origin()+ray.direction()*hit.minT();
  Vector normal;
  hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
  Vector phongnormal = normal;
  double costheta = -Dot(normal, ray.direction());
  double eta_inverse;
  bool exiting;
  if(costheta < 0){
    // Exiting surface
    normal = -normal;
    costheta = -costheta;
    eta_inverse = eta;
    exiting = true;
  } else {
    // Entering surface
    eta_inverse = inv_eta;
    exiting = false;
  }

  Vector refl_dir = ray.direction()+2*costheta*normal;
  double costheta2squared = 1 + (costheta*costheta-1)*(eta_inverse*eta_inverse);
  if(costheta2squared < 0){
    // Total internal reflection...
    Ray refl_ray(hitpos, refl_dir);
    double t_refl;
    Color refl_color;
    if(nothing_inside)
      t_refl = scene->traceRay(refl_color, context, hit.getPrimitive(), refl_ray, atten, depth+1);
    else
      t_refl = scene->traceRay(refl_color, context, refl_ray, atten, depth+1);
    if(exiting){
      Color attenuation(exp(extinction_constant.r()*t_refl), exp(extinction_constant.g()*t_refl),
                        exp(extinction_constant.g()*t_refl));
      result = refl_color * attenuation;
    } else {
      result = refl_color;
    }
  } else {
    double costheta2 = sqrt(costheta2squared);
    double k = 1-Min(costheta, costheta2);
    double k2 = k*k;
    double k4 = k2*k2;
    double k5 = k4*k;
    double refl = r0*(1-k5)+k5;

    // Reflective ray
    Color refl_color;
    Ray refl_ray(hitpos, refl_dir);
    double t_refl = scene->traceRay(refl_color, context, refl_ray, atten*refl, depth+1);
    if(exiting){
      Color attenuation(exp(extinction_constant.r()*t_refl), exp(extinction_constant.g()*t_refl),
                        exp(extinction_constant.g()*t_refl));
      result = refl_color * attenuation*refl;
    } else {
      result = refl_color*refl;
    }
    
    // Transparency ray
    Vector transp_dir = eta_inverse*ray.direction()+(eta_inverse*costheta-costheta2)*normal;
    Color transp_color;
    Ray transp_ray(hitpos, transp_dir);
    double t_trans = scene->traceRay(transp_color, context, transp_ray, atten*(1-refl), depth+1);
    if(!exiting){
      Color attenuation(exp(extinction_constant.r()*t_trans), exp(extinction_constant.g()*t_trans),
                        exp(extinction_constant.g()*t_trans));
      result += transp_color * attenuation*(1-refl);
    } else {
      result += transp_color*(1-refl);
    }
  }

  const Object* world = scene->getObject();
  Light*const* begin = &lights[0];
  Light*const* end = &lights[lights.size()];
  Color light(0, 0, 0);
  while(begin != end){
    Color light_color;
    Vector light_direction;
    double dist = (*begin++)->getLight(light_color, light_direction, context, hitpos);
    double cosphi = Dot(phongnormal, light_direction);
    // Uncomment the following conditional to get the assignment on the
    // web page
    if(/* !exiting && */ cosphi > 0){
      // Cast shadow rays...
      HitRecord shadowhit(dist);
      Ray shadowray(hitpos, light_direction);
      world->intersect(shadowhit, context, shadowray);
      if(!shadowhit.getPrimitive()){
        // No shadows...
        Vector H;
        if(exiting)
          H = light_direction + ray.direction();
        else
          H = light_direction - ray.direction();
        H.normalize();
        double cos_alpha = Dot(H, phongnormal);
        if(cos_alpha > 0){
          double phong_term = ipow(cos_alpha, phong_exponent);
          result += light_color*phong_term;
        }
      }
    }
  }
}
