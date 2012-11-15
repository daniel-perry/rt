
#ifndef PhongMaterial_h
#define PhongMaterial_h

#include "Material.h"
#include "Color.h"

class PhongMaterial : public Material {
 public:
  PhongMaterial(const Color& color, float Kd, float Ka, const Color& phong_color, int phong_exponent);
  virtual ~PhongMaterial();

  virtual void shade(Color& result, const RenderContext& context, const Ray& ray,
                     const HitRecord& hit, const Color& atten, int depth) const;

 private:
  PhongMaterial(const PhongMaterial&);
  PhongMaterial& operator=(const PhongMaterial&);

  Color color;
  float Kd;
  float Ka;
  Color phong_color;
  int phong_exponent;
};

#endif
