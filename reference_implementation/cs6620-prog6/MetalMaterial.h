
#ifndef MetalMaterial_h
#define MetalMaterial_h

#include "Material.h"
#include "Color.h"

class MetalMaterial : public Material {
 public:
  MetalMaterial(const Color& refl, int phong_exponent);
  virtual ~MetalMaterial();

  virtual void shade(Color& result, const RenderContext& context, const Ray& ray,
                     const HitRecord& hit, const Color& atten, int depth) const;

 private:
  MetalMaterial(const MetalMaterial&);
  MetalMaterial& operator=(const MetalMaterial&);

  Color refl;
  Color not_refl;
  int phong_exponent;
};

#endif
