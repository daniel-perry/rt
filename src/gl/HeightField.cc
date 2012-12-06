/* Daniel Perry
 * for cs6620 - spring 2005
 * started 8feb05
 */


/* HeightField: height field class.
 */

using namespace std;

#include "HeightField.h"
#include "util.h"
#include <math.h>
#include <fstream>

#include <iostream>
#include <cstdlib>

#include "BBox.h"

BBox HeightField::getBBox(){
  return BBox( bounds[0] , bounds[1] );
}

void HeightField::readHF(const char * hf_filename){
  ifstream file( hf_filename );
  if (!file){
    cerr << "Error opening " << hf_filename << endl;
    std::exit(1);
  }
  //double minz, maxz;
  file >> nx >> ny >> dmin >> dmax;
  if (!file){
    cerr << "Error reading header from " << hf_filename << endl;
    std::exit(1);
  }
  file.get();
  data = new float*[nx+1];
  float * p = new float[ (nx+1)*(ny+1) ];
  for( int i=0; i<=nx; i++)
    data[i] = p+i*(ny+1);
  file.read( (char*)data[0] , sizeof(float)*(nx+1)*(ny+1));
  if(!file){
    cerr << "Error reading data from "<<hf_filename<<endl;
    std::exit(1);
  }

  cerr<<"What was read in:"<<endl;
  cerr<<"nx:"<<nx<<" ny:"<<ny<<" dmin:"<<dmin<<" dmax:"<<dmax<<endl;
  /*for(int i=0;i<=nx;i++){
    for(int j=0;j<=ny;j++){
      cerr<<data[i][j]<<", ";
    }
    cerr<<endl;
  }
  */

  Pmin.setz( dmin - 1e-4 );
  Pmax.setz( dmax + 1e-4 );
  bounds[0] = Pmin;
  bounds[1] = Pmax;
}

bool HeightField::intersect( HitRecord & hit , const RenderContext & context, const ray & r ){
  double t0 = MYMIN , t1 = MYMAX;
  double tmin,tmax,tymin,tymax,tzmin,tzmax;
  vector3d n_min,n_max;
  
  tmin = (bounds[r.sign[0]].x() - r.origin().x()) * r.inv_d.x();
  tmax = (bounds[1-r.sign[0]].x() - r.origin().x()) * r.inv_d.x();
  tymin = (bounds[r.sign[1]].y() - r.origin().y()) * r.inv_d.y();
  tymax = (bounds[1-r.sign[1]].y() - r.origin().y()) * r.inv_d.y();

  if( (tmin>tymax) || (tymin > tmax) ){
    return false;
  }
  if( tymin > tmin ){
    tmin = tymin;
      n_min = vector3d(0,-1,0);
  }else{
      n_min = vector3d(-1,0,0);
  }
  if( tymax < tmax ){
    tmax = tymax;
    n_max = vector3d(0,1,0);
  }else{
    n_max = vector3d(1,0,0);
  }
  tzmin = (bounds[r.sign[2]].z() - r.origin().z()) * r.inv_d.z();
  tzmax = (bounds[1-r.sign[2]].z() - r.origin().z()) * r.inv_d.z();
  if( (tmin > tzmax) || (tzmin > tmax) ){
    return false;
  }
  if( tzmin > tmin ){
    tmin = tzmin;
    n_min = vector3d(0,0,-1);
  }
  if( tzmax < tmax ){
    tmax = tzmax;
    n_max = vector3d(0,0,1);
  }

  if( (tmin < t1 ) && (tmax > t0) ){
    vector3d checker(r.sign[0],r.sign[1],r.sign[2]); // to reverse normals if ray approaches with negative slope.
    checker*=-1;
    checker += vector3d( 1-r.sign[0], 1-r.sign[1], 1-r.sign[2] ); // should end up with checker looking like (1,-1,1) depending on whether slope was negative or positive in (x,y,z)

    n_min *= checker;
    n_max *= checker;

    //Normal = n_min;

    
    // Now tnear = tmin and tfar = tmax
    double tnear = tmin, tfar = tmax;
    if( tnear < 0 ){
      tnear =0;
    }

    //    return hit.hit(tnear, this, material);

    // compute lattice coordinates.
    vector3d worldSpace = r.eval( tnear );
    int Lx = (int)((worldSpace.x()-Pmin.x())/cellsize.x());
    int Ly = (int)((worldSpace.y()-Pmin.y())/cellsize.y());
    //int Lz = (int)((worldSpace.z()-Pmin.z())/cellsize.z());
    if(Lx<0)Lx=0;
    else if(Lx>=nx)Lx=nx-1;
    if(Ly<0)Ly=0;
    else if(Ly>=ny)Ly=ny-1;
    
    // find how ray marching changes index
    int diy;// = ((D.y()*r.direction().y())>0)?1:-1;
    int stopy;// = ((D.y()*r.direction().y())>0)?ny:-1;
    //cerr<<"if statement:"<<((D.y()*r.direction().y())>0)<<endl;
    if( (D.y()*r.direction().y())>0 ){
      diy=1;
      stopy=ny;
    }else{
      diy=-1;
      stopy=-1;
    }

    int dix;// = ((D.x()*r.direction().x())>0)?1:-1;
    int stopx;// = ((D.x()*r.direction().x())>0)?nx:-1;
    if((D.x()*r.direction().x())>0){
      dix = 1;
      stopx = nx;
    }else{
      dix = -1;
      stopx = -1;
    }

    // find how t changes with ray marching
    double dtdx = fabs( cellsize.x()/r.direction().x());
    double dtdy = fabs( cellsize.y()/r.direction().y());

    vector3d far;

    // determine the far edges of the cell:
    if( dix == 1){
      far.setx( (Lx+1) * cellsize.x() + Pmin.x() );
    }else{
      far.setx( Lx * cellsize.x() + Pmin.x() );
    } 

    if( diy == 1){
      far.sety( (Ly+1) * cellsize.y() + Pmin.y() );
    }else{
      far.sety( Ly * cellsize.y() + Pmin.y() );
    }

    double tnext_x, tnext_y;

    // determine t value of far slabs
    tnext_x = (far.x()-r.origin().x())/r.direction().x();
    tnext_y = (far.y()-r.origin().y())/r.direction().y();

    // beginning of loop
    double zenter,texit,zexit,zmin,zmax;  //zmintemp, zmaxtemp;
    float datamin,datamax;
    vector3d C,S,D,E,EC;
    double Zc, Zu, Zv, Zuv,a,b,c,t2,t3;
    while(!((Lx==stopx || Ly==stopy) || (tnear > tfar) )){
      // computer range of z vals
      zenter = r.origin().z()+tnear*r.direction().z();
      texit = fmin( tnext_x , tnext_y );
      zexit = r.origin().z()+texit*r.direction().z();
      
      datamin = fmin( fmin(data[Lx][Ly],data[Lx+1][Ly]) , fmin(data[Lx][Ly+1],data[Lx+1][Ly+1]));
      zmin = fmin(zenter,zexit);
      
      datamax = fmax( fmax(data[Lx][Ly],data[Lx+1][Ly]) , fmax(data[Lx][Ly+1],data[Lx+1][Ly+1]));
      zmax = fmax( zenter,zexit);
      
      //zmintemp = zmin-Pmin.z();
      ///zmaxtemp = zmax-Pmin.z();
      if(!(zmin>datamax || zmax<datamin)){	
	// calc intersect with cell.
	//return hit.hit(tnear,this,material);
	// using a bilinear patch made of the data pts:
	
	//first find C:
	C.setz(0);
	C.setx( Pmin.x() + cellsize.x()*Lx);
	C.sety( Pmin.y() + cellsize.y()*Ly);
	
	//find E, S, D:
	E = r.eval( tnear );
	EC = E-C;
	S.setx( EC.x() / cellsize.x() );
	S.sety( EC.y() / cellsize.y() );
	D.setx( r.direction().x() / cellsize.x() );
	D.sety( r.direction().y() / cellsize.y() );
	
	Zc = data[Lx][Ly];
	Zu = data[Lx+1][Ly] - data[Lx][Ly];
	Zv = data[Lx][Ly+1] - data[Lx][Ly];
	Zuv = data[Lx+1][Ly+1] - data[Lx+1][Ly] - data[Lx][Ly+1] + data[Lx][Ly];

	a = D.x()*D.y()*Zuv;
	b = D.x()*Zu + D.y()*Zv + D.y()*S.x()*Zuv + D.x()*S.y()*Zuv - r.direction().z();
	c = Zc + Zu*S.x()+S.y()*Zv + Zuv*S.x()*S.y() - E.z();

	double tnext_xy = fmin(tnext_x,tnext_y);
	
	if(fabs(a)<MYMIN){ // solve lin equation:
	  t2 = -c/b;
	  t2+=tnear;

	  // check to see if correct:
	  if( (t2 > (tnear + MYMIN)) && (t2 < (tnext_xy+MYMIN)) ){
	    if(hit.hit(t2, Normal, this , material )){
	      S = r.eval( t2 );//P
	      
	      a = (S.x()-C.x())/cellsize.x();//u
	      b = (S.y()-C.y())/cellsize.y();//v
	      a = fmin( 0. , fmax( 1. , a ) );
	      b = fmin( 0. , fmax( 1. , b ) );
	      Normal = vector3d(-((Zu+b*Zuv)/cellsize.x()),-((Zv+a*Zuv)/cellsize.y()),1);
	      Normal.MakeUnitVector();
	      return true;
	    }else{
	      return false;
	    }
	  }
	  
	}
	else{ // solve using quad equation:
	  double det = sqrt( b*b - 4*a*c );
	  t2 = (-b + det ) / (2*a);
	  t3 = (-b - det ) / (2*a);
	  t2+=tnear;
	  t3+=tnear;
	  
	  // check to see which is correct.
	  if((t2 > (tnear + MYMIN)) && (t2 < (tnext_xy+MYMIN))){
	    if((t3 > (tnear + MYMIN)) && (t3 < (tnext_xy+MYMIN))){
		t2 = fmin(t2,t3);	      
	      if(hit.hit( t2, Normal, this , material )){
		S = r.eval(t2);//P
		
		a = (S.x()-C.x())/cellsize.x();//u
		b = (S.y()-C.y())/cellsize.y();//v
		a = fmin( 0. , fmax( 1. , a ) );
		b = fmin( 0. , fmax( 1. , b ) );
		Normal = vector3d(-((Zu+b*Zuv)/cellsize.x()),-((Zv+a*Zuv)/cellsize.y()),1);
		Normal.MakeUnitVector();
		return true;
	      }
	      else{
		return false;
	      }
	    }
	    if(hit.hit(t2, Normal, this , material )){
	      S = r.eval( t2 );//P
	      
	      a = (S.x()-C.x())/cellsize.x();//u
	      b = (S.y()-C.y())/cellsize.y();//v
	      a = fmin( 0. , fmax( 1. , a ) );
	      b = fmin( 0. , fmax( 1. , b ) );
	      Normal = vector3d(-((Zu+b*Zuv)/cellsize.x()),-((Zv+a*Zuv)/cellsize.y()),1);
	      Normal.MakeUnitVector();
	      return true;
	    }else{
	      return false;
	    }	    
	  }
	  else if((t3 > (tnear + MYMIN)) && (t3 < (tnext_xy+MYMIN))){	
	    if(hit.hit(t3, Normal, this , material )){
	      S = r.eval( t3 );//P
	      
	      a = (S.x()-C.x())/cellsize.x();//u
	      b = (S.y()-C.y())/cellsize.y();//v
	      a = fmin( 0. , fmax( 1. , a ) );
	      b = fmin( 0. , fmax( 1. , b ) );
	      Normal = vector3d(-((Zu+b*Zuv)/cellsize.x()),-((Zv+a*Zuv)/cellsize.y()),1);
	      Normal.MakeUnitVector();
	      return true;
	    }else{
	      return false;
	    }	    
	  }
	}
	

      }
      
      // march to next cell
      if( tnext_x < tnext_y){
	tnear = tnext_x;
	tnext_x += dtdx;
	Lx += dix;
      }else{
	tnear = tnext_y;
	tnext_y += dtdy;
	Ly += diy;
      }
    }
    
  }
  return false;
}

