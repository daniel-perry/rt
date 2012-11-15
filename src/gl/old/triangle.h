#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "surface.h"
#include "rgb.h"
#include "vector3d.h"
#include "hpoint.h"

class triangle : public surface

{
 public:
  //triangle();
  triangle( const hpoint & np0 = hpoint(0,0,0,1), 
	    const hpoint & np1 = hpoint(1,0,0,1), 
	    const hpoint & np2 = hpoint(1,1,0,1), 
	    const rgb & kd = rgb(1,1,1), 
	    const rgb & ks = rgb(1,1,1), 
	    double kh = 0.0, 
	    double e = 1.0,
	    texture * t = 0)
  {
    init( np0,np1,np2,kd,ks,kh,e,t );
  }

  triangle( const vector3d & np0, 
	    const vector3d & np1, 
	    const vector3d & np2, 
	    const rgb & kd = rgb(1,1,1), 
	    const rgb & ks = rgb(1,1,1), 
	    double kh = 0.0, 
	    double e = 1.0,
	    texture * t = 0)
  {
    init( hpoint(np0,1),hpoint(np1,1),hpoint(np2,1),kd,ks,kh,e,t );
  }
  
  triangle( const triangle & nt )
  { 
    init( nt.hp0(), nt.hp1(),nt.hp2(),nt.matte(),nt.mirror(),nt.highlight(),nt.phongexponent(), nt.myTexture);
  }

  //doing this in surface.h
  //~triangle()
  //{
  //  delete myTexture;
  //}

  // used to initialize the class - constructors call it.
  void init( const hpoint & np0, 
	    const hpoint & np1, 
	    const hpoint & np2, 
	    const rgb & kd, 
	    const rgb & ks, 
	    double kh, 
	    double e,
	    texture * t)
  {
    myP0 = np0;
    myP1 = np1;
    myP2 = np2;
    setmatte( kd );
    setmirror( ks );
    sethighlight( kh );
    setphongexponent( e );
    
    myTexture = t;
  }

  triangle operator=(const triangle & nt )
  {
    init( nt.hp0(), nt.hp1(),nt.hp2(),nt.matte(),nt.mirror(),nt.highlight(),nt.phongexponent(), nt.myTexture);
    return (*this);
  }

  vector3d p0() const
  {
    return vector3d(myP0.x(),myP0.y(),myP0.z());
  }
  vector3d p1() const
  {
    return vector3d(myP1.x(),myP1.y(),myP1.z());
  }
  vector3d p2() const
  {
    return vector3d(myP2.x(),myP2.y(),myP2.z());
  }
  
  hpoint hp0() const
  {
    return myP0;
  }
  hpoint hp1() const
  {
    return myP1;
  }
  hpoint hp2() const
  {
    return myP2;
  }

  void setp0( const vector3d & np0 )
  {
    myP0 = hpoint(np0,1);
  } 
  void setp1( const vector3d & np1 )
  {
    myP1 = hpoint(np1,1);
  } 
  void setp2( const vector3d & np2 )
  {
    myP2 = hpoint(np2,1);
  }

  void setp0( const hpoint & np0 )
  {
    myP0 = np0;
  } 
  void setp1( const hpoint & np1 )
  {
    myP1 = np1;
  } 
  void setp2( const hpoint & np2 )
  {
    myP2 = np2;
  }

  vector3d normal() const
  {
    vector3d temp = cross( p1()-p0() , p2()-p0() );
    return temp/temp.length();
  }

  bool hit (const ray & r, double tmin, double tmax, double & t, surfacevals & sv ) const; 

  vector3d centroid() const;
  dynarray<vector3d> elements() const;

  //private:
  hpoint myP0, myP1, myP2;
};

#endif




