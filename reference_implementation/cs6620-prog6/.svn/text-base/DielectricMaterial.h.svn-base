
#ifndef DielectricMaterial_h
#define DielectricMaterial_h

#include "Material.h"
#include "Color.h"

class DielectricMaterial : public Material {
 public:
  DielectricMaterial(double eta, int phong_exponent);
  DielectricMaterial(double eta, const Color& extinction, int phong_exponent);
  virtual ~DielectricMaterial();

  virtual void shade(Color& result, const RenderContext& context, const Ray& ray,
                     const HitRecord& hit, const Color& atten, int depth) const;

 private:
  DielectricMaterial(const DielectricMaterial&);
  DielectricMaterial& operator=(const DielectricMaterial&);

  double eta;
  double inv_eta;
  double r0;
  Color extinction;
  Color extinction_constant;
  int phong_exponent;
  bool nothing_inside;
};

#endif
