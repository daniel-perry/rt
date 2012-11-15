
#include "Ring.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Math.h"
#include "Ray.h"

Ring::Ring(Material* material, const Point& center, const Vector& normal, double r1, double r2)
  : Primitive(material), center(center), n(normal)
{
  n.normalize();
  d = Dot(n, center);
  double inner = Min(r1, r2);
  double outer = Max(r1, r2);
  inner_radius2 = inner*inner;
  outer_radius2 = outer*outer;
}

Ring::~Ring()
{
}

void Ring::getBounds(BoundingBox& bbox) const
{
  double radius = sqrt(outer_radius2);
  Vector diag(radius*sqrt(n.y()*n.y()+n.z()*n.z()),
              radius*sqrt(n.x()*n.x()+n.z()*n.z()),
              radius*sqrt(n.x()*n.x()+n.y()*n.y()));
  bbox.extend(center+diag);
  bbox.extend(center-diag);
}

void Ring::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
  double denom = Dot(n, ray.direction());
  if(Abs(denom) > 1.e-6){
    double t = (d-Dot(n, ray.origin()))/denom;
    Point hitpos = ray.origin()+ray.direction()*t;
    Vector r = hitpos-center;
    double l = r.length2();
    if(l < outer_radius2 && l > inner_radius2)
      hit.hit(t, this, matl);
  }
}

void Ring::normal(Vector& normal, const RenderContext&,
                   const Point&, const Ray&, const HitRecord&) const
{
  normal = n;
}
