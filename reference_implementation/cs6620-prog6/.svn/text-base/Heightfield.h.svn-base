
#ifndef Heightfield_h
#define Heightfield_h

#include "Primitive.h"
#include "Point.h"
#include <string>
class Ray;

class Heightfield : public Primitive {
 public:
  Heightfield(Material* material, const std::string& filename,
              const Point& p1, const Point& p2);
  virtual ~Heightfield();

  virtual void getBounds(BoundingBox& bbox) const;
  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
  virtual void normal(Vector& normal, const RenderContext& context,
                      const Point& hitpos, const Ray& ray, const HitRecord& hit) const;
 private:
  Point p1, p2;
  float** data;
  int nx, ny;
  Vector diag;
  Vector cellsize;
  Vector inv_cellsize;

  struct HitData {
    int x, y;
  };
};

#endif
