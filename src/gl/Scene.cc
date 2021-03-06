/* Daniel Perry
 * for cs6620 - spring 2005
 * started 31jan05
 * cc started 4 mar 05
 */

/* Scene: holds all the elements of a scene.
 *
 *
 */

#include "Scene.h"
#include "HitRecord.h"
#include "Object.h"

void Scene::render( Color & clr , const ray & r ) const {
  HitRecord hit(m_hit);
  RenderContext rc(m_rc);

  hit.t = MYMAX;
  hit.bbox_t = MYMAX;
  if( object->intersect( hit , rc , r ) ){
    hit.material->shade( clr , rc , r , hit , 0 , 1 );
    //hit.material->shade( clr , rc , r , hit , MaxRayDepth , MinAttenuation );
  }else{
    background->getBackgroundColor( clr , rc , r );
  }
}
