
#include "Disk.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Math.h"
#include "Ray.h"

Disk::Disk(Material* material, const Point& center, const Vector& normal, double radius)
  : Primitive(material), center(center), n(normal), radius2(radius*radius)
{
  n.normalize();
  d = Dot(n, center);
}

Disk::~Disk()
{
}

void Disk::getBounds(BoundingBox& bbox) const
{
  double radius = sqrt(radius);
  Vector diag(radius*sqrt(n.y()*n.y()+n.z()*n.z()),
              radius*sqrt(n.x()*n.x()+n.z()*n.z()),
              radius*sqrt(n.x()*n.x()+n.y()*n.y()));
  bbox.extend(center+diag);
  bbox.extend(center-diag);
}

void Disk::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
  double denom = Dot(n, ray.direction());
  if(Abs(denom) > 1.e-6){
    double t = (d-Dot(n, ray.origin()))/denom;
    Point hitpos = ray.origin()+ray.direction()*t;
    Vector r = hitpos-center;
    if(r.length2() < radius2)
      hit.hit(t, this, matl);
  }
}

void Disk::normal(Vector& normal, const RenderContext&,
                   const Point&, const Ray&, const HitRecord&) const
{
  normal = n;
}
