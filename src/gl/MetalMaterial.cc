/* Daniel Perry
 * for cs6620 - spring 2005
 * started 14feb05
 */

/* MetalMaterial: material phong shading model.with reflectance to look like metal.
 *
 *
 */


#include "MetalMaterial.h"
#include "util.h"
#include "Primitive.h"
#include "Scene.h"

void MetalMaterial::shade( rgb & result, const RenderContext & context, const ray & r, const HitRecord & hit, int depth , double attenuation) const{
  vector3d pt = r.eval( hit.t );
  vector3d norm = hit.prim->normal(pt);
  HitRecord tempHit;
  rgb speclight(0,0,0);

  double cosTheta = dot( norm , r.direction());
  if( cosTheta > 0 ){
    norm = -norm;
  }else{
    cosTheta = -cosTheta;
  }

  for( int i=0; i<(int)context.scene->lights.size() ; i++){
    rgb lightColor;
    vector3d lightDir;
    vector3d lightPos;
    double cosPhi, distance;
    distance = context.scene->lights[i]->getLight( lightColor , lightDir , context , pt );    
#ifdef _TWO_SIDED_LIGHTING_OP
    cosPhi = fabs(dot( lightDir , norm ));
#else
    cosPhi = dot( lightDir , norm );
#endif

    if( cosPhi > 0 ){
      tempHit.t = distance;
      if( !context.scene->object->intersect(tempHit,context,ray(pt,lightDir)) ){
	
	vector3d Half = (lightDir - r.direction());
	Half.MakeUnitVector();
	double cosAlpha = dot( Half , norm );
	if( cosAlpha > 0 ){
	  speclight+=(lightColor*pow(cosAlpha,Exponent));
	}

      }
    }

  }
  result = Reflective * speclight;

  if( depth < context.scene->MaxRayDepth ){
    double cT = (1-cosTheta);
    rgb F = Reflective + ((rgb(1,1,1) - Reflective)*cT*cT*cT*cT*cT);
    vector3d refDir = r.direction()+2*cosTheta*norm;
    
    rgb refClr;
    tempHit.t = MYMAX;
    ray refRay(pt,refDir);
    if(context.scene->object->intersect(tempHit,context,refRay)){
      tempHit.material->shade( refClr , context , refRay , tempHit , depth+1 , attenuation);
    }else{
      context.scene->background->getBackgroundColor( refClr , context , refRay );
    }
    result += F*refClr;

  }

}
