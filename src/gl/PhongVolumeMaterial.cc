/* Daniel Perry
 * for cs6620 - spring 2005
 * started 28mar05
 */

/* PhongVolumeMaterial: volume rendering using the Phong lighting model.
 */

#include "PhongVolumeMaterial.h"
#include "HitRecord.h"
#include "Primitive.h"
#include "Scene.h"

#include <math.h>
#include <fstream>
#include <cstdlib>
using namespace std;

void PhongVolumeMaterial::preprocess(){}


void PhongVolumeMaterial::shade( rgb & result, const RenderContext & context, const ray & r, HitRecord & hit, int depth, double attenuation) const{

  // find texit:
  double t_enter = hit.t, t_exit=t_enter;
  vector3d safety_offset(1e-6,1e-6,1e-6);
  ray r_inside( r.eval( hit.t ) + safety_offset , r.direction() );
  HitRecord hit_inside;
  hit_inside.t = MYMAX;
  
  hit.prim->intersect( hit_inside , context , r_inside );
  if( hit_inside.t < MYMAX ){ // hit other side
    t_exit = t_enter+hit_inside.t;
  }
  /*// t_exit has same value as t_enter.
  else{ // didn't hit (ie original hit point was on edge of box)    
  }
  */

  double it = ceil( t_enter / grid_stepsize);
  double t = it * grid_stepsize;
  std::cerr << "t_enter: " << t_enter << std::endl;
  std::cerr << "stepsize: " << grid_stepsize << std::endl;
  std::cerr << "it: " << it << std::endl;
  std::cerr << "t: " << t << std::endl;

  Color accum_color(0,0,0);
  double accum_opacity = 0;  

  /*********************************************
    may need to calculate own diag for box object (not material class's) to convert P to lattice coordinates below.

    ALSO - check that floor() from math.h floors negative numbers correctly!!!
  **********************************************/
  //cerr<<endl<<endl<<"new t val"<<endl<<endl;
  while( t < t_exit ){
    //cerr<<"t:"<<t<<" t_exit:"<<t_exit<<endl;
    Point P = r.eval( t );
    //cerr<<"P:"<<P<<" lower:"<<lower<<endl;
    P = P - lower;
    //cerr<<"P-lower"<<P<<endl;
    P = P / diag;
    //cerr<<"P/diag:"<<P<<endl;
    P = P * vector3d( (size1-1) , (size2-1) , (size3-1) );
    //cerr<<"P*sizes:"<<P<<endl;
    Point P_lower = vector3d( floor( P.x() ) , floor( P.y() ) , floor( P.z() ) );
    Point P_diff = P - P_lower;
   
    //cerr<<"P_lower:"<<P_lower<<" P_diff:"<<P_diff<<endl;
    //cerr<<"P:"<<P<<endl;
    //cerr<<"r.eval(t_exit)->converted:"<< ((r.eval(t_exit)-lower)/diag) * vector3d( (size1-1) , (size2-1) , (size3-1) ) <<endl;

    int lower_int[3] = { ((int)P_lower.x()) , ((int)P_lower.y()) , ((int)P_lower.z()) };
    // now P in lattice coordinates, need to interpolate:

    //first get value at current cell, and next cell...
    //GOING TO NEED TO GET DATA POINTS AT ALL 4 POINTS ARROUND CENTER POINT
    // TO BE ABLE TO DO TRI-LINEAR INTERPOLATION...
    
    
    //cerr<<"here1"<<endl;

    if( lower_int[0] == (size1-1))
      lower_int[0]--;
    if(lower_int[1] == (size2-1))
      lower_int[1]--;
    if(lower_int[2] == (size3-1))
      lower_int[2]--;
    /*
    cerr <<"here1: size1,2,3:"<<size1<<","<<size2<<","<<size3<<endl
	 <<"lower_int[0,1,2]:"<<lower_int[0]<<","<<lower_int[1]<<","<<lower_int[2]<<endl;
    */
    double v000 = data[lower_int[0]][lower_int[1]][lower_int[2]],
           v100 = data[lower_int[0]+1][lower_int[1]][lower_int[2]],
           v110 = data[lower_int[0]+1][lower_int[1]+1][lower_int[2]],
           v111 = data[lower_int[0]+1][lower_int[1]+1][lower_int[2]+1],
           v010 = data[lower_int[0]][lower_int[1]+1][lower_int[2]],
           v011 = data[lower_int[0]][lower_int[1]+1][lower_int[2]+1],
           v001 = data[lower_int[0]][lower_int[1]][lower_int[2]+1],
           v101 = data[lower_int[0]+1][lower_int[1]][lower_int[2]+1];

    //cerr<<"here1.5"<<endl;

    double value = v000 * (1-P_diff.x())*(1-P_diff.y())*(1-P_diff.z()) +
                   v100 * P_diff.x() * (1-P_diff.y())*(1-P_diff.z()) +
                   v010 * (1-P_diff.x()) * P_diff.y() * (1-P_diff.z()) +
                   v001 * (1-P_diff.x()) * (1-P_diff.y()) * P_diff.z() +
                   v101 * P_diff.x() * (1-P_diff.y()) * P_diff.z() +
                   v011 * (1-P_diff.x()) * P_diff.y() * P_diff.z() +
                   v110 * P_diff.x() * P_diff.y() * (1-P_diff.z()) +
                   v111 * P_diff.x() * P_diff.y() * P_diff.z();
    
    //cerr<<"here1.75"<<endl;
    float opacity;
    Color color;
    cmap.lookup( (float)value , opacity , color );
    
    if( opacity > 0 ){ // shade
      

      //cerr<<"here2"<<endl;
      ////////////////
      //COMPUTE NORMAL      
      Vector norm;
      if( lower_int[0] == 0 ){
	norm.setx( ( v100 - v000) * cellsize.x() );
      }
      else if( lower_int[0] == (size1-1) ){
	norm.setx( (v000 - data[lower_int[0]-1][lower_int[1]][lower_int[2]] ) * cellsize.x() );
      }
      else{
	norm.setx( (v100 -  data[lower_int[0]-1][lower_int[1]][lower_int[2]] )*2* cellsize.x() );
      }

      if( lower_int[1] == 0 ){
	norm.sety( ( v010 - v000) * cellsize.y() );
      }
      else if( lower_int[1] == (size2-1) ){
	norm.sety( (v000 - data[lower_int[0]][lower_int[1]-1][lower_int[2]] ) * cellsize.y() );
      }
      else{
	norm.sety( (v010 -  data[lower_int[0]][lower_int[1]-1][lower_int[2]] )*2* cellsize.y() );
      }

      if( lower_int[2] == 0 ){
	norm.setz( ( v001 - v000) * cellsize.z() );
      }
      else if( lower_int[2] == (size3-1) ){
	norm.setz( (v000 - data[lower_int[0]][lower_int[1]][lower_int[2]-1] ) * cellsize.z() );
      }
      else{
	norm.setz( (v001 -  data[lower_int[0]][lower_int[1]][lower_int[2]-1] )*2* cellsize.z() );
      }
      
      if( norm.squaredLength() == 0 ){
	norm = - r.direction();
      }

      norm.MakeUnitVector();


      //
      ///////////////////////////

      //cerr<<"here3"<<endl;

      ///////////////////////////
      //BEGIN PHONG LIGHTING: (from PhongMaterial.cc)
      Vector pt = r.eval( t );

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
	   //tempHit.t = distance;
	   //no shadows:
	   //if( !context.scene->object->intersect(tempHit,context,ray(pt,lightDir)) ){
	     light += costheta * Kd  * lightColor;
	
	     // Phong Highlight contribution.
	     vector3d Half = (lightDir - r.direction());
	     Half.MakeUnitVector();
	     double cosAlpha = dot( Half , norm );
	     if( cosAlpha > 0 ){
	       speclight+=(lightColor*pow(cosAlpha,phong_exponent));
	       }
	     
	     //}
	 }
       }
       //END PHONG LIGHTING ...
       /////////////////////////////

       color = color * light + speclight * phong_color;
    }
    
    accum_color += color * opacity * (1-accum_opacity);
    accum_opacity += opacity*(1-accum_opacity);

    t += world_stepsize;
    
  }
 
  // volume should be "see-through", so now going to cast ray behind it...

  

    //(may need to add safety small vector to new_r origin.....)

    ray new_r( r.eval( t_exit ) , r.direction() );
    Color exit_color(0,0,0);

    context.scene->render( exit_color , new_r );

    accum_color += exit_color * (1-accum_opacity);
  
  
 
  result = accum_color;

  //cerr<<"leaving volume::shade function"<<endl;

}



/* The following two functions were given to the class from
 * the instructor and TA.
*/

void swap(short& data)
{
  char* p=(char*)&data;
  char tmp=p[0];
  p[0]=p[1];
  p[1]=tmp;
}

PhongVolumeMaterial::PhongVolumeMaterial(const std::string& headername,
                                         const std::string& cmapname,
                                         const Point& lower, const Point& upper,
                                         double grid_stepsize, float maxopacity,
                                         float Kd, float Ka,
                                         const Color& phong_color,
                                         int phong_exponent)
  : cmap(cmapname), lower(lower), upper(upper), grid_stepsize(grid_stepsize),
    maxopacity(maxopacity), Kd(Kd), Ka(Ka), phong_color(phong_color),
    phong_exponent(phong_exponent)
{
  diag = upper-lower;
  bounds[0] = lower;
  bounds[1] = upper;

  ifstream hdr(headername.c_str());
  string volumename;
  hdr >> volumename;
  int nx, ny, nz;
  hdr >> nx >> ny >> nz;
  short offsetvalue;
  hdr >> offsetvalue;
  string fileendian;
  hdr >> fileendian;
  if(!hdr){
    cerr << "Error reading header: " << headername << '\n';
    exit(1);
  }
  if(fileendian != "BigEndian" && fileendian != "LittleEndian"){
    cerr << "Bad data endian: " << fileendian << " in " << headername << "\n";
    exit(1);
  }
  cerr << "Reading " << volumename << ": " << nx << 'x' << ny << 'x' << nz << '\n';
  
  //SETUP ARRAY TO HOLD DATA VALS AS SHORT's
  //data.resize(nx, ny, nz);
  size1 = nx;
  size2 = ny;
  size3 = nz;
  data = new short**[size1];
  for(int i=0;i<size1;i++){
    data[i] = new short*[size2];
  }
  for( int i=0;i<size1;i++){
    for(int j=0;j<size2;j++){
      data[i][j] = new short[size3];
    }
  }


  cellsize = diag * Vector(1./(nx-1), 1./(ny-1), 1./(nz-1));
  ifstream in(volumename.c_str()); //
  in.read(reinterpret_cast<char*>(&data[0][0][0]), nx*ny*nz*sizeof(short));

  if(!in){
    cerr << "Error reading data: " << volumename << '\n';
    exit(1);
  }

  world_stepsize = cellsize.length()/pow(3, 1./3.) * grid_stepsize;
  cmap.rescale(world_stepsize);

  short tmp = 0x1234;
  string machineendian;
  if(*reinterpret_cast<char*>(&tmp) == 0x12)
    machineendian = "BigEndian";
  else
    machineendian = "LittleEndian";

  cerr<<"here1"<<endl;

  if(machineendian != fileendian){
    for(int i=0;i<nz;i++){
      for(int j=0;j<ny;j++){
        for(int k=0;k<nx;k++){
          swap(data[k][j][i]);
          data[k][j][i] += offsetvalue;
        }
      }
    }
  } else {
    for(int i=0;i<nz;i++){
      for(int j=0;j<ny;j++){
        for(int k=0;k<nx;k++){
          data[k][j][i] += offsetvalue;
        }
      }
    }
  }
  
  cerr<<"leaving PhongVolumeMaterial constructor."<<endl;

}
