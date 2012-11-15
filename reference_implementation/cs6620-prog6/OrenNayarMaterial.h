
#ifndef OrenNayarMaterial_h
#define OrenNayarMaterial_h

#include "Material.h"
#include "Color.h"

class OrenNayarMaterial : public Material {
 public:
  OrenNayarMaterial(const Color& color, double sigma);
  virtual ~OrenNayarMaterial();

  virtual void shade(Color& result, const RenderContext& context, const Ray& ray,
                     const HitRecord& hit, const Color& atten, int depth) const;

 private:
  OrenNayarMaterial(const OrenNayarMaterial&);
  OrenNayarMaterial& operator=(const OrenNayarMaterial&);

  Color color;
  double A, B;
};

#endif
