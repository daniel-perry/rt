
#include "Triangle.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Math.h"
#include "Ray.h"

Triangle::Triangle(Material* material, const Point& p1, const Point& p2, const Point& p3)
  : Primitive(material), p1(p1), p2(p2), p3(p3)
{
  e1 = p1-p3;
  e2 = p2-p3;
  n = Cross(e1, e2);
  n.normalize();
}

Triangle::~Triangle()
{
}

void Triangle::getBounds(BoundingBox& bbox) const
{
  bbox.extend(p1);
  bbox.extend(p2);
  bbox.extend(p3);
}

void Triangle::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
  Vector r1 = Cross(ray.direction(), e2);
  double denom = Dot(e1, r1);
  if(Abs(denom) < 1.e-8)
    return;
  double invDenom = 1./denom;
  Vector s = ray.origin()-p3;
  double b1 = Dot(s, r1)*invDenom;
  if(b1 < 0 || b1 > 1)
    return;
  Vector r2 = Cross(s, e1);
  double b2 = Dot(ray.direction(), r2)*invDenom;
  if(b2 < 0 || b1+b2 >1)
    return;
  double t = Dot(e2, r2)*invDenom;
  hit.hit(t, this, matl);
}

void Triangle::normal(Vector& normal, const RenderContext&,
                   const Point&, const Ray&, const HitRecord&) const
{
  normal = n;
}
