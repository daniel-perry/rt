
#include "Box.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Math.h"
#include "Point.h"
#include "Ray.h"
#include "Vector.h"
#include <iostream>
using namespace std;

Box::Box(Material* material, const Point& p1, const Point& p2)
  : Primitive(material)
{
  min = Min(p1, p2);
  max = Max(p1, p2);
}

Box::~Box()
{
}

void Box::getBounds(BoundingBox& bbox) const
{
  bbox.extend(min);
  bbox.extend(max);
}

void Box::intersect(HitRecord& hit, const RenderContext&, const Ray& ray) const
{
  Vector idir = ray.direction().inverse();
  Vector v1 = (min-ray.origin())*idir;
  Vector v2 = (max-ray.origin())*idir;
  Vector vmin = Min(v1, v2);
  Vector vmax = Max(v1, v2);
  double near = vmin.maxComponent();
  double far = vmax.minComponent();
  //cerr << this << " min=" << min << ", max=" << max << '\n';
  //cerr << this << " v1=" << v1 << ", v2=" << v2 << '\n';
  //cerr << this << " orig=" << ray.origin() << ", dir=" << ray.direction() << '\n';
  //cerr << this << " vmin=" << vmin << ", near=" << near << ", vmax" << vmax << ", far=" << far << '\n';
  if(near < far){
    if(!hit.hit(near, this, matl)){
      hit.hit(far, this, matl);
    }
  }
}

void Box::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                    const Ray& ray, const HitRecord& hit) const
{
  if(Abs(hitpos.x()-min.x()) < 1.e-6)
    normal = Vector(-1,0,0);
  else if(Abs(hitpos.x()-max.x()) < 1.e-6)
    normal = Vector(1,0,0);
  else if(Abs(hitpos.y()-min.y()) < 1.e-6)
    normal = Vector(0,-1,0);
  else if(Abs(hitpos.y()-max.y()) < 1.e-6)
    normal = Vector(0,1,0);
  else if(Abs(hitpos.z()-min.z()) < 1.e-6)
    normal = Vector(0,0,-1);
  else
    normal = Vector(0,0,1);
}
