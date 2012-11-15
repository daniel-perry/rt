/* Daniel Perry
 * for cs6620 - spring 2005
 * started 4 April 2005
 */

/* TInstance - transformed instance class
 *
 */

#include "TInstance.h"
#include "hpoint.h"
#include "Material.h"
#include "matrix.h"

TInstance::TInstance( Primitive * o , Material * mat , const matrix & m ):original(o),transform_matrix(m),material(mat){
  inverse_transform_matrix = transform_matrix.inverse();
}

bool TInstance::intersect( HitRecord & hit , const RenderContext & context, const ray & r ){
  ray trans_r( transform_point_inverse(r.origin()) , transform_vector_inverse( r.direction() ).MakeUnitVector() );

  if( original -> intersect( hit , context , trans_r ) ){
    Vector hit_p = trans_r.eval(hit.t);
    //hit_p = transform_point( hit_p );
    hit.setPoint(transform_point( hit_p ));
    Normal = original -> normal( hit_p );
    Normal = transform_normal( Normal );
    Normal.MakeUnitVector();
    hit.prim = this;
    hit.material = material;
    
    return true;
  }
  
  return false;
  
}

vector3d TInstance::normal( const vector3d & p ) const{
  return Normal;
}


Vector TInstance::transform_point( const Vector & p ){
  hpoint pt( p , 1 );
  pt = transform_matrix * pt;
  return Vector( pt.x() , pt.y() , pt.z() );
}
Vector TInstance::transform_vector( const Vector & v ){
  hpoint pt( v , 0 );
  pt = transform_matrix * pt;
  return Vector( pt.x() , pt.y() , pt.z() );  
}
Vector TInstance::transform_normal( const Vector & n ){
  hpoint pt( n , 0 );
  pt = inverse_transform_matrix.transpose() * pt;
  return Vector( pt.x() , pt.y() , pt.z() );    
}

Vector TInstance::transform_point_inverse( const Vector & p ){
  hpoint pt( p , 1 );
  pt = inverse_transform_matrix * pt;
  return Vector( pt.x() , pt.y() , pt.z() );
}
Vector TInstance::transform_vector_inverse( const Vector & v ){
  hpoint pt( v , 0 );
  pt = inverse_transform_matrix * pt;
  return Vector( pt.x() , pt.y() , pt.z() );  
}
Vector TInstance::transform_normal_inverse( const Vector & n ){
  hpoint pt( n , 0 );
  pt = transform_matrix.transpose() * pt;
  return Vector( pt.x() , pt.y() , pt.z() );    
}
