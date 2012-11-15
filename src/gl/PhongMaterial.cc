/* Daniel Perry
 * for cs6620 - spring 2005
 * started 12feb05
 */

/* PhongMaterial: material using the phong shading model.(provides for phong highlights).
 *
 *
 */

#include "PhongMaterial.h"
#include "util.h"
#include "Primitive.h"
#include <math.h>
#include "Scene.h"

void PhongMaterial::shade( rgb & result, const RenderContext & context, const ray & r, HitRecord & hit, int depth, double attenuation) const{
  HitRecord tempHit;
  tempHit.id = 2;
  //vector3d pt  = r.eval( hit.t );
  vector3d pt  = hit.hitPoint( hit.t , r );
  vector3d norm = hit.prim->normal(pt);
  double cosPhi = -dot( norm , r.direction() );
  if( cosPhi < 0 ){
    cosPhi = -cosPhi;
    norm = -norm;
  }
  rgb light = context.scene->AmbientLight * Ka;
  rgb speclight(0,0,0);
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
	
	// Phong Highlight contribution.
	vector3d Half = (lightDir - r.direction());
	Half.MakeUnitVector();
	double cosAlpha = dot( Half , norm );
	if( cosAlpha > 0 ){
	  speclight+=(lightColor*pow(cosAlpha,Exponent));
	}
	
	}
    }
  }
  result = light * matte + speclight * reflective;
}
