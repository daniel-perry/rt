

#include "CoupledMaterial.h"
#include "util.h"
#include "Primitive.h"
#include "Scene.h"

void CoupledMaterial::shade( rgb & result, const RenderContext & context, const ray & r, const HitRecord & hit, int depth, double attenuation) const{

  HitRecord tempHit;
  vector3d pt = r.eval( hit.t );
  vector3d norm = hit.prim->normal(pt);

  double cosTheta = -dot( norm , r.direction() );
  
  double Fr = 1-cosTheta;
  Fr = Fr*Fr*Fr*Fr*Fr;
  double Ft = 1-Fr;

  rgb light/*(0,0,0);*/ = context.scene->AmbientLight * Ka;
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
	double cosPhi = dot( norm , lightDir );
	double Fr1 = 1-cosPhi;
	Fr1 = Fr1*Fr1*Fr1*Fr1*Fr1;
	double Ft1 = 1-Fr1;
	

	light += Ft*Ft1* lightColor *Kd;


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
  
  result = Surface * light ;

  // reflective contribution next:

  if( depth < context.scene->MaxRayDepth ){

    vector3d refDir = r.direction()+2*cosTheta*norm;
    
    rgb refClr;
    tempHit.t = MYMAX;
    ray refRay(pt,refDir);
    if(context.scene->object->intersect(tempHit,context,refRay)){
      tempHit.material->shade( refClr , context , refRay , tempHit , depth+1 , attenuation);
    }else{
      context.scene->background->getBackgroundColor( refClr , context , refRay );
    }
    result += Fr*refClr;

  }

  
  // add in Phong Highlight:
  result+= Phong*speclight;
    
}
