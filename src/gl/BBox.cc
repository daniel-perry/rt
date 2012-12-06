/* Daniel Perry
 * for cs6620 - spring 2005
 * started 22mar05
 */

/* BoundBox: bounding box to be used in a bounding volume hierarchy acceleration structure.
 */

#include "BBox.h"
#include "vector3d.h"

BBox::BBox(){}
BBox::BBox( const Point & min_pt , const Point & max_pt ){
  bounds[0] = min_pt;
  bounds[1] = max_pt;
}

Point BBox::centroid(){
  return (bounds[0]+bounds[1])/2.;
}

BBox BBox::getBBox(){ return (*this);}

void BBox::preprocess(){}

BBox merge ( const BBox & b1 , const BBox & b2 ){
  BBox bbox;

  bbox.bounds[0] = Point( fmin( b1.bounds[0].x() , b2.bounds[0].x() ) ,
			  fmin( b1.bounds[0].y() , b2.bounds[0].y() ) ,
			  fmin( b1.bounds[0].z() , b2.bounds[0].z() ) );

  
  bbox.bounds[1] = Point( fmax( b1.bounds[1].x() , b2.bounds[1].x() ) ,
			  fmax( b1.bounds[1].y() , b2.bounds[1].y() ) ,
			  fmax( b1.bounds[1].z() , b2.bounds[1].z() ) );

  return bbox;
}


// same as axis box, but without normal calculations.
bool BBox::intersect( HitRecord & hit , const RenderContext & context , const ray & r ) const {
   double t0 = MYMIN , t1 = MYMAX;
  double tmin,tmax,tymin,tymax,tzmin,tzmax;
  //vector3d n_min,n_max;

  tmin = (bounds[r.sign[0]].x() - r.origin().x()) * r.inv_d.x();
  tmax = (bounds[1-r.sign[0]].x() - r.origin().x()) * r.inv_d.x();
  tymin = (bounds[r.sign[1]].y() - r.origin().y()) * r.inv_d.y();
  tymax = (bounds[1-r.sign[1]].y() - r.origin().y()) * r.inv_d.y();

  if( (tmin>tymax) || (tymin > tmax) ){
    return false;
  }
  if( tymin > tmin ){
    tmin = tymin;
    // n_min = vector3d(0,-1,0);
  }/*else{
      n_min = vector3d(-1,0,0);
      }*/
  if( tymax < tmax ){
    tmax = tymax;
    //n_max = vector3d(0,1,0);
  }/*else{
    n_max = vector3d(1,0,0);
    }*/
  tzmin = (bounds[r.sign[2]].z() - r.origin().z()) * r.inv_d.z();
  tzmax = (bounds[1-r.sign[2]].z() - r.origin().z()) * r.inv_d.z();
  if( (tmin > tzmax) || (tzmin > tmax) ){
    return false;
  }
  if( tzmin > tmin ){
    tmin = tzmin;
    //n_min = vector3d(0,0,-1);
  }
  if( tzmax < tmax ){
    tmax = tzmax;
    //n_max = vector3d(0,0,1);
  }

  if( (tmin < t1 ) && (tmax > t0) ){
    //vector3d checker(r.sign[0],r.sign[1],r.sign[2]); // to reverse normals if ray approaches with negative slope.
    //checker*=-1;
    //checker += vector3d( 1-r.sign[0], 1-r.sign[1], 1-r.sign[2] ); // should end up with checker looking like (1,-1,1) depending on whether slope was negative or positive in (x,y,z)

    // ie : checker = <1,1,1> - 2 * <sign>
    //n_min *= checker;
    //n_max *= checker;

    //Normal = n_min;

    if(tmin < MYMIN ){ // if in center of box, this will return the one infront....
      tmin = tmax;
      //Normal = n_max;
    }
    return hit.hit_bbox( tmin ,0 , 0 ) ;
  }
  return false;
}

