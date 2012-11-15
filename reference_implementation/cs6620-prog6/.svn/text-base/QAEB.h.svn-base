
#ifndef QAEB_h
#define QAEB_h

#include "Primitive.h"
#include "Point.h"
class Ray;

template<typename Func> class QAEB : public Primitive {
 public:
  QAEB(Material* material, const Point& p1, const Point& p2, double epsilon);
  virtual ~QAEB();

  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
  virtual void normal(Vector& normal, const RenderContext& context,
                      const Point& hitpos, const Ray& ray, const HitRecord& hit) const;
 private:
  Point min, max;
  double epsilon;
};


#include "Box.h"
#include "HitRecord.h"
#include "Math.h"
#include "Point.h"
#include "Ray.h"
#include "Vector.h"

template<typename Func>
QAEB<Func>::QAEB(Material* material, const Point& p1, const Point& p2, double epsilon)
  : Primitive(material), epsilon(epsilon)
{
  min = Min(p1, p2);
  max = Max(p1, p2);
}

template<typename Func>
QAEB<Func>::~QAEB()
{
}

template<typename Func>
void QAEB<Func>::intersect(HitRecord& hit, const RenderContext&, const Ray& ray) const
{
  Vector idir = ray.direction().inverse();
  Vector v1 = (min-ray.origin())*idir;
  Vector v2 = (max-ray.origin())*idir;
  Vector vmin = Min(v1, v2);
  Vector vmax = Max(v1, v2);
  double near = vmin.maxComponent();
  double far = vmax.minComponent();
  if(near > far || far < 1.e-6)
    return;
  if(near<epsilon){
    near=epsilon;
    return;
  }
  far = Min(far, hit.minT());
  double t = near;
  Func f;
  double h = max.z()-min.z();
  double m = min.z();
  Point p = ray.origin() + t*ray.direction();
  double oz = p.z()-(f(p.x(), p.y())*h+m);
  Vector d = ray.direction()*epsilon;
  while(t < far){
    double nz = p.z()-(f(p.x(), p.y())*h+m);
    if(oz*nz<0){
      // Hit at t...
      double frac = oz/(oz-nz);
      t = t*(1+epsilon*frac)/(1+epsilon);
      hit.hit(t, this, matl);
      return;
    }
    p += d*t;
    t += epsilon*t;
    oz=nz;
  }
}

template<typename Func>
void QAEB<Func>::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                        const Ray& ray, const HitRecord& hit) const
{
  Func f;
  double e = (hit.minT()*epsilon)*0.5;
  //double e = 0.5;
  double dx = f(hitpos.x()+e, hitpos.y())-f(hitpos.x()-e, hitpos.y());
  double dy = f(hitpos.x(), hitpos.y()+e)-f(hitpos.x(), hitpos.y()-e);
  double h = max.z()-min.z();
  Vector vx(2*e, 0, dx*h);
  Vector vy(0, 2*e, dy*h);
  normal = Cross(vx, vy);
  normal.normalize();
}


#endif
