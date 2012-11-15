/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 */

/* LambertianMaterial: material using the lambertian shading model.
 *
 *
 */
#include "LambertianMaterial.h"
#include "Primitive.h"
#include "util.h"
#include "Scene.h"

void LambertianMaterial::shade( rgb & result, const RenderContext & context, const ray & r, const HitRecord & hit, int depth, double attenuation) const{
  HitRecord tempHit;
  tempHit.id = 2;
  vector3d pt  = r.eval( hit.t );
  vector3d norm = hit.prim->normal(pt);
#if 1
  double cosPhi = -dot( norm , r.direction() );
  if( cosPhi < 0 ){
    cosPhi = -cosPhi;
    norm = -norm;
  }
#endif
  rgb light = context.scene->AmbientLight * Ka;
  for(int i =0 ; i< (int)context.scene->lights.size() ; i++){
    rgb lightColor;
    vector3d lightDir;
    vector3d lightPos;
    double costheta,distance;
    distance = context.scene->lights[i]->getLight( lightColor , lightDir , context , pt );
    #ifdef _TWO_SIDED_LIGHTING_OP
    costheta = fabs(dot( lightDir , norm ));
    #else
    costheta = dot( lightDir , norm );
    #endif
    
    if( costheta > 0 ){
      tempHit.t = distance;
      if( !context.scene->object->intersect(tempHit,context,ray(pt,lightDir)) ){
	light += costheta * Kd  * lightColor;
      }
    }
  }
  result = light * matte;
}
