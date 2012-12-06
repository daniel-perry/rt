/* Daniel Perry
 * for cs6620 - spring 2005
 * started 14feb05
 */

/* DielectMaterial: transparent material (glass, clear plastic, etc.)
 *
 *
 */

#include "DielectricMaterial.h"
#include "util.h"
#include "Primitive.h"
#include "Scene.h"

void DielectricMaterial::shade( rgb & result, const RenderContext & context, const ray & r, const HitRecord & hit, int depth, double attenuation) const{
  
  HitRecord tempHit;
  vector3d pt = r.eval( hit.t );
  //vector3d norm = hit.prim->normal( pt );
  vector3d norm = hit.normal;
  double cosTheta = dot( norm , r.direction() );
  vector3d unflippedNorm = norm;
  double RefTmp;
  rgb speclight(0,0,0);

  if( cosTheta > 0 ){
    RefTmp = 1/Refraction;
    norm = -norm;
  }else{
    RefTmp = Refraction;
    cosTheta = -cosTheta;
  }
  
  // compute phong term with unflipped norm.
  for( int i=0; i<(int)context.scene->lights.size() ; i++){
    rgb lightColor;
    vector3d lightDir;
    vector3d lightPos;
    double cosPhi, distance;
    distance = context.scene->lights[i]->getLight( lightColor , lightDir , context , pt );    
    /*#ifdef _TWO_SIDED_LIGHTING_OP
    cosPhi = fabs(dot( lightDir , unflippedNorm ));
    #else*/
    cosPhi = dot( lightDir , unflippedNorm );
    //#endif

    if( cosPhi > 0 ){
      tempHit.t = distance;
      if( !context.scene->object->intersect(tempHit,context,ray(pt,lightDir)) ){
	
	vector3d Half = (lightDir - r.direction());
	Half.MakeUnitVector();
	double cosAlpha = dot( Half , unflippedNorm );
	if( cosAlpha > 0 ){
	  speclight+=(lightColor*pow(cosAlpha,Exponent));
	}
      }
    }    
  }
  // end phong term.

  result = speclight*rgb(1,1,1);

  if( depth  >= context.scene->MaxRayDepth || attenuation < context.scene->MinAttenuation){
    result = rgb(0,0,0);
  }else{
    
    double cosTheta2 = 1+((cosTheta*cosTheta-1)/(RefTmp*RefTmp));

    if( cosTheta2 < 0 ){ // total internal reflection, trace reflection just like in metal.

      vector3d refDir = r.direction()+2*cosTheta*norm;      
      refDir.MakeUnitVector();
      rgb refClr;
      tempHit.t = MYMAX;
      ray refRay(pt,refDir);
      if(context.scene->object->intersect(tempHit,context,refRay)){
	tempHit.material->shade( refClr , context , refRay , tempHit , depth+1 , attenuation);
      }else{
	context.scene->background->getBackgroundColor( refClr , context , refRay );
      }
      result += refClr;
      
    }else{
      
      cosTheta2 = sqrt( cosTheta2 );
      double cosm = fmin( cosTheta , cosTheta2 );

      double cT = (1-cosm);
      double F = R + ((1-R)*cT*cT*cT*cT*cT);
      
      // trace reflection ray just like metal, do we use cosm or cosTheta?
      vector3d refDir = r.direction()+2*cosTheta*norm;
      refDir.MakeUnitVector();
      rgb refClr;
      tempHit.t = MYMAX;
      ray refRay(pt,refDir);
      if(context.scene->object->intersect(tempHit,context,refRay)){
	tempHit.material->shade( refClr , context , refRay , tempHit , depth+1 , attenuation*F);
      }else{
	context.scene->background->getBackgroundColor( refClr , context , refRay );
      }
      result += refClr*F;

      //now do transparency ray
      
      double Ft = 1-F;

      vector3d trDir = (1/RefTmp)*r.direction() + (((cosTheta/RefTmp)-cosTheta2)*norm);
      //cerr<<"trDir:"<<trDir<<" len:"<<trDir.length()<<endl;
      //trDir.MakeUnitVector();
      ray trRay( pt , trDir );
      tempHit.t = MYMAX;
      rgb trClr;
      if(context.scene->object->intersect(tempHit,context,trRay)){
	tempHit.material->shade( trClr , context , trRay , tempHit , depth+1 , attenuation*Ft);
      }else{
	context.scene->background->getBackgroundColor( trClr , context , trRay );
      }
      
      result += trClr*Ft;
      

    }

  }


}
