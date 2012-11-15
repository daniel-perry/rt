/* Daniel Perry
 * for cs6620 - spring 2005
 * started 4 April 2005
 */

/* TInstance - transformed instance class
 *
 */


#ifndef _TINSTANCE_H
#define _TINSTANCE_H

#include "Primitive.h"
#include "matrix.h"
#include "BBox.h"

class TInstance : public Primitive
{
 public:
  TInstance( Primitive * o , Material * mat , const matrix & m );

  bool intersect( HitRecord & hit , const RenderContext & context, const ray & r );
  // normal returned is guaranteed to be normalized.
  vector3d normal( const vector3d & p ) const;

  void preprocess(){}
  BBox getBBox(){return original->getBBox();}
  char* print(){ return "TInstance";}


  Vector transform_point( const Vector & p );
  Vector transform_vector( const Vector & v );
  Vector transform_normal( const Vector & n );

  Vector transform_point_inverse( const Vector & p );
  Vector transform_vector_inverse( const Vector & v );
  Vector transform_normal_inverse( const Vector & n );


  Primitive * original;
  Vector Normal;
  matrix transform_matrix,inverse_transform_matrix;
  Material * material;
  
 private:

};
#endif
