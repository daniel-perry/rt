
#ifndef Box_h
#define Box_h

#include "Primitive.h"
#include "Point.h"
class Ray;

class Box : public Primitive {
 public:
  Box(Material* material, const Point& p1, const Point& p2);
  virtual ~Box();

  virtual void getBounds(BoundingBox& bbox) const;
  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
  virtual void normal(Vector& normal, const RenderContext& context,
                      const Point& hitpos, const Ray& ray, const HitRecord& hit) const;
 private:
  Point min, max;
};

#endif
