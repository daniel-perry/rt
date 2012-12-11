/* Daniel Perry
 * for cs6620 - spring 2005
 * started 8feb05
 */


/* HeightField: height field class.
 */


#ifndef _HEIGHTFIELD_H
#define _HEIGHTFIELD_H

#include "Primitive.h"

class HeightField : public Primitive
{
 public:
  HeightField(const vector3d& pmin, const vector3d & pmax , const char * hf_filename, Material * matl){
    bounds[0] = Pmin = pmin;
    bounds[1] = Pmax = pmax;
    material = matl;
    readHF( hf_filename );
    
    preprocess();
  }
  
  HeightField( Material * matl, const char * hf_filename, const vector3d& pmin, const vector3d & pmax){
    bounds[0] = Pmin = pmin;
    bounds[1] = Pmax = pmax;
    material = matl;
    readHF( hf_filename );
    
    preprocess();
  }

  void readHF(const char * hf_filename);

  bool intersect( HitRecord & hit , const RenderContext & context, const ray & r ) const;
  
  // normal returned is guaranteed to be normalized.
  vector3d normal( const vector3d & p ) const{
    return Normal;
  }

  std::string toString(){return "HeightField";}

  void preprocess(){
    // find diagnol
    D = Pmax-Pmin;

    // find cell size:
    //cellsize = D / vector3d(nx-.001,ny-.001,1);
    cellsize = D / vector3d(nx,ny,1);

    // find min and max z vals
    /*
    Zmin = Pmin.z();
    if( Pmax.z() < Zmin )
      Zmin = Pmax.z();
    Zmax = Pmax.z();
    if( Pmin.z() > Zmax )
      Zmax = Pmin.z();
    */

    // find min and max of data.
    /*
    dmax = dmin = data[0][0];
    for(int i=1; i<nx+1; i++){
      for(int j=0;j<ny+1; j++){
	if(data[i][j] > dmax)
	  dmax = data[i][j];
	if(data[i][j] < dmin)
	  dmin = data[i][j];
      }
    }
    */
    
  }

  BBox getBBox();

  vector3d Pmin, Pmax, D, cellsize;
  //double Zmin, Zmax;
  int nx,ny; // cell resolution;
  float **data;
  float dmin,dmax;
  Material * material;
  
  //borrowed from AxisBox:
  vector3d Normal;
  vector3d bounds[2];

 private:
  HeightField(){}

};

typedef HeightField Heightfield;

#endif
