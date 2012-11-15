#include "image.h"
#include <stdlib.h>  // for abs()
#include <math.h> // for ceil(), floor()
#include <values.h> // for MAXFLOAT
//#include <limits.h> //for INT_MAX
#include <string>

using namespace std;

#define MY_MAX_FLOAT MAXFLOAT

image::image() 
  : imageWidth(0), imageHeight(0)
{
  raster = 0;
  zbuffer = 0;
}

image::image( int w, int h ) 
  : imageWidth(w), imageHeight(h)
{
  zbuffer = new (float*)[h];
  raster = new ( rgb* )[ h ];
  for (int r=0; r<h; r++)
  {
    raster[r] = new rgb[ w ];
    zbuffer[r] = new float[w];
    for (int c=0; c<w; c++)
      zbuffer[r][c] = -MY_MAX_FLOAT;
  }
}

image::~image()
{
  for (int r=0; r < height(); r++)
  {
    delete [] raster[r];
    delete [] zbuffer[r];
  }
  delete [] raster;
  delete [] zbuffer;
}


const image & image::operator = ( const image & im )
{  
  int t_height = height();
  rgb ** temp_raster = raster;
  float ** temp_zb = zbuffer;

  imageWidth = im.width();
  imageHeight = im.height();
  raster = new (rgb*)[height()];
  zbuffer = new (float*)[height()];

  for (int r=0 ; r < height() ; r++ )
  {
    raster[r] = new rgb[width()];
    zbuffer[r] = new float[width()];

    for (int c=0 ; c < width() ; c++)
    {
      raster[r][c] = im.raster[r][c];
      zbuffer[r][c] = im.zbuffer[r][c];
    }
  }

  for (int r=0; r < t_height; r++)
  {
    delete [] temp_raster[r];
    delete [] temp_zb[r];
  }
  delete [] temp_raster;
  delete [] temp_zb; 

  return (*this);
}

int image::width() const
{
  return imageWidth;
}

int image::height() const
{
  return imageHeight;
}


void image::resetraster( int w , int h )
{
  for (int r=0; r < height(); r++)
  {
    delete [] raster[r];
    delete [] zbuffer[r];
  }
  delete [] raster;
  delete [] zbuffer;

  imageWidth = w;
  imageHeight = h;
  
  zbuffer = new (float*)[h];
  raster = new ( rgb* )[ h ];
  for (int r=0; r<h; r++)
  {
    raster[r] = new rgb[ w ];
    zbuffer[r] = new float[w];
    for (int c=0; c<w; c++)
      zbuffer[r][c] = -MY_MAX_FLOAT;
  }

}

void image::antialias()
{
  image aa_img( width()-1 , height()-1 );
  for( int y=0; y<aa_img.height() ; y++ )
  {
    for( int x=0 ; x<aa_img.width() ; x++ )
    {
      aa_img.set( x , y , (get(x,y)+get(x+1,y)+get(x,y+1)+get(x+1,y+1))/4.0 );
    }
  }
  
  (*this) = aa_img;
}

void image::drawline( int x0, int y0, rgb c0, int x1, int y1, rgb c1 )
{
  if ( abs( y1-y0 ) <= abs( x1-x0 ) )  // slope = [-1,1]
  {
    if (x1<x0)
    {
      int temp=x0;
      x0=x1;
      x1=temp;
      temp=y0;
      y0=y1;
      y1=temp;
      
      rgb tempc = c0;
      c0 = c1;
      c1 = tempc;
    }
    
    float dx = x1-x0, dy = (y1-y0)/dx, y = y0;
    rgb c = c0, dc = (c1-c0)/dx;

    for ( int x=x0; x<=x1; x++)
    {
      set( x, int(y+.5), c );
      y+=dy;
      c+=dc;
    } 
    
  }
  else  // slope != [-1,1]
  {
    if (y1<y0)
    {
      int temp=x0;
      x0=x1;
      x1=temp;
      temp=y0;
      y0=y1;
      y1=temp;
      
      rgb tempc = c0;
      c0 = c1;
      c1 = tempc;
    }
    float dy = y1-y0, dx = (x1-x0)/dy, x = x0 ;
    rgb c = c0, dc = (c1-c0)/dy;

    for ( int y = y0; y<=y1; y++ )
    {
      set( int(x+.5) , y , c );
      x+=dx;
      c+=dc;
    }
    
  }
}

void image::drawtriangle( float x0, float y0, rgb c0, float x1, float y1, rgb c1, float x2, float y2, rgb c2 )
{
  int xmin = (int)ceil(minNum( minNum( x0 , x1 ) , x2 )),
      xmax = (int)floor(maxNum( maxNum( x0 , x1 ) , x2 )),
      ymin = (int)ceil(minNum( minNum( y0 , y1 ) , y2 )),
      ymax = (int)floor(maxNum( maxNum( y0 , y1 ) , y2 ));
  float alpha, beta, gamma, 
        falpha = f_ab( x1, y1, x2, y2, x0, y0 ), 
        fbeta = f_ab( x2, y2, x0, y0, x1, y1 ), 
        fgamma = f_ab( x0, y0, x1, y1, x2, y2 );  

  rgb c;
  
  for (int y = ymin; y <= ymax; y++)
  {
    for (int x = xmin; x<= xmax; x++)
    {
      alpha = f_ab( x1, y1, x2, y2, x, y ) / falpha;
      beta = f_ab( x2, y2, x0, y0, x, y ) / fbeta;
      gamma = f_ab( x0, y0, x1, y1, x, y ) / fgamma;
      
      if ( alpha >= 0 && beta >= 0 && gamma >= 0 )
	if( (alpha > 0 || falpha * f_ab(x1,y1,x2,y2,-1,-1)>0 ) && ( beta>0 || fbeta*f_ab(x2,y2,x0,y0,-1,-1)>0 ) && ( gamma>0 || fgamma * f_ab(x0,y0,x1,y1,-1,-1)>0 ) )
	{
	  c = alpha * c0 + beta * c1 + gamma * c2;
	  set( x , y , c );
	}
    }
  }
}

//this one is for using a zbufffer
void image::drawtriangle( float x0, float y0, float zbv0, rgb c0, float x1, float y1, float zbv1, rgb c1, float x2, float y2, float zbv2, rgb c2 )
{
  int xmin = (int)ceil(minNum( minNum( x0 , x1 ) , x2 )),
      xmax = (int)floor(maxNum( maxNum( x0 , x1 ) , x2 )),
      ymin = (int)ceil(minNum( minNum( y0 , y1 ) , y2 )),
      ymax = (int)floor(maxNum( maxNum( y0 , y1 ) , y2 ));
  float alpha, beta, gamma, 
        falpha = f_ab( x1, y1, x2, y2, x0, y0 ), 
        fbeta = f_ab( x2, y2, x0, y0, x1, y1 ), 
        fgamma = f_ab( x0, y0, x1, y1, x2, y2 );  

  rgb c;
  
  for (int y = ymin; y <= ymax; y++)
  {
    for (int x = xmin; x<= xmax; x++)
    {
      alpha = f_ab( x1, y1, x2, y2, x, y ) / falpha;
      beta = f_ab( x2, y2, x0, y0, x, y ) / fbeta;
      gamma = f_ab( x0, y0, x1, y1, x, y ) / fgamma;
      
      if ( alpha >= 0 && beta >= 0 && gamma >= 0 )
	if( (alpha > 0 || falpha * f_ab(x1,y1,x2,y2,-1,-1)>0 ) && ( beta>0 || fbeta*f_ab(x2,y2,x0,y0,-1,-1)>0 ) && ( gamma>0 || fgamma * f_ab(x0,y0,x1,y1,-1,-1)>0 ) )
	{
	  c = alpha * c0 + beta * c1 + gamma * c2;
	  set( x , y , c , alpha*zbv0+beta*zbv1+gamma*zbv2 );
	}
    }
  }
}

// this one is to make implementing texture mapping easier.
void image::drawtriangle( const triangle & t)
{
  float x0=t.hp0().x(), y0=t.hp0().y(), zbv0=t.hp0().z(),
        x1=t.hp1().x(), y1=t.hp1().y(), zbv1=t.hp1().z(),
        x2=t.hp2().x(), y2=t.hp2().y(), zbv2=t.hp2().z();
  rgb c0 = t.matte(), c1 = t.matte(), c2 = t.matte();

  int xmin = (int)ceil(minNum( minNum( x0 , x1 ) , x2 )),
      xmax = (int)floor(maxNum( maxNum( x0 , x1 ) , x2 )),
      ymin = (int)ceil(minNum( minNum( y0 , y1 ) , y2 )),
      ymax = (int)floor(maxNum( maxNum( y0 , y1 ) , y2 ));
  float alpha, beta, gamma, 
        falpha = f_ab( x1, y1, x2, y2, x0, y0 ), 
        fbeta = f_ab( x2, y2, x0, y0, x1, y1 ), 
        fgamma = f_ab( x0, y0, x1, y1, x2, y2 );  

  rgb c;
  
  for (int y = ymin; y <= ymax; y++)
  {
    for (int x = xmin; x<= xmax; x++)
    {
      alpha = f_ab( x1, y1, x2, y2, x, y ) / falpha;
      beta = f_ab( x2, y2, x0, y0, x, y ) / fbeta;
      gamma = f_ab( x0, y0, x1, y1, x, y ) / fgamma;
      
      if ( alpha >= 0 && beta >= 0 && gamma >= 0 )
	if( (alpha > 0 || falpha * f_ab(x1,y1,x2,y2,-1,-1)>0 ) && ( beta>0 || fbeta*f_ab(x2,y2,x0,y0,-1,-1)>0 ) && ( gamma>0 || fgamma * f_ab(x0,y0,x1,y1,-1,-1)>0 ) )
	{
	  if (t.myTexture!=0)
	  {
	    c = t.myTexture->color(vector3d(beta,gamma,0));
	    cerr<<"in image.drawtriangle(triangle):"<<endl
		<<"successfully called t.myTexture->color(...)"<<endl;
	  }
	  else
	  {
	    c = alpha * c0 + beta * c1 + gamma * c2;
	  }
	  
	  set( x , y , c , alpha*zbv0+beta*zbv1+gamma*zbv2 );
	}
    }
  }
}

void image::readppm ( istream & s )
{
  string st;
  int w,h;
  double maxval;
  rgb temp;
  s >> st >> w >> h;
  resetraster(w,h);

  s >> maxval;
  
  // character - ppm
  if ( st == "P3" )
  {
    for (int r=height()-1; r>=0; r--)
    { 
      for (int c=0; c<width(); c++)
      {
	s >> temp[0] >> temp[1] >> temp[2];
	temp /= maxval;
	temp *= temp;  // because of sqrt call in writeppm
	set( c , r , temp );
      }
    }
  }
  // bit-ppm
  else if ( st == "P6" )
  {
    for (int r=height()-1; r>=0; r--)
    { 
      for (int c=0; c<width(); c++)
      {
	temp.setr((unsigned int)s.get());
	temp.setg((unsigned int)s.get());
	temp.setb((unsigned int)s.get());
	temp /= maxval;
	temp *= temp;  // because of sqrt() call in writeppmbit
	set( c , r , temp );
      }
    }
  }
}

// writes the contents of raster to a file in ppm text format
void image::writeppm( ostream & s )
{
  s<<"P3\n"<<width()<<" "<<height()<<"\n255\n";
  for (int r=height()-1; r>=0; r--)
  { 
    for (int c=0; c<width(); c++)
    {
      s << toInt( get(c,r).r() )<<" ";
      s << toInt( get(c,r).g() )<<" ";
      s << toInt( get(c,r).b() )<<" ";
    }
    s << "\n";
  }
}

//preconditon:
//postcondition: writes the contents of raster to a file in ppm bit format
void image::writeppmBit( ostream & s )
{
  s<<"P6\n"<<width()<<" "<<height()<<"\n255\n";
  
  for (int r=height()-1; r>=0; r--)
    for (int c=0; c<width(); c++)
    {
      s.put( (unsigned char) toInt( get(c,r).r() ) );
      s.put( (unsigned char) toInt( get(c,r).g() ) );
      s.put( (unsigned char) toInt( get(c,r).b() ) );
    }
}

//precondition: f represents an r, g, or b color value, preferably on interval [0,1).
//postcondition: returns the integer equivelent on interval [0,255).
unsigned int image::toInt( float f ) const
{
  unsigned int i = ( unsigned int )( 256 * sqrt(f) );
  if (i > 255) i = 255;
  return i;
}

// returns the rgb value at (x,y)
const rgb & image::get( int x, int y) const
{
  // check for out-of-bounds values
  assert ( x >= 0 && x < width() );
  assert ( y >= 0 && y < height() );

  cerr<<"image::get("<<x<<','<<y<<")"<<endl<<width()<<","<<height()<<endl;

  return raster[y][x];
}

//this one is for using a zbuffer
void image::set( int x, int y, rgb c, float zb_val )
{
  if ( !(x >= 0 && x < width()) || !( y >= 0 && y < height() ) ) //clipping
  {
    //cerr<<"coudn't plot ("<<x<<","<<y<<"): outside of image"<<endl;
    return;
  }
  
  if ( zb_val >= zbuffer[y][x] )
  {
    raster[y][x] = c;
    zbuffer[y][x] = zb_val;
  }
}

// sets pixel (x,y) on image to the rgb c.
// note: (0,0) is the left-bottom pixel.
void image::set( int x, int y, rgb c)
{ 
  if ( !(x >= 0 && x < width()) || !( y >= 0 && y < height() ) ) //clipping
  {
    //cerr<<"coudn't plot ("<<x<<","<<y<<"): outside of image"<<endl;
    return;
  }
  // check for out-of-bounds values
  assert ( x >= 0 && x < width() );
  assert ( y >= 0 && y < height() );
  

  

  raster[y][x] = c;
}


/* insted of [r][c] use get(x,y) and set(x,y,rgb).

//precondition: r >=0 and < height()  - check that in the function
//postcondition: returns that row of rgb's
//NOTE: usually you'd use this like so, image1[r][c];  however, there is no checking for out of bounds in built-in c++ arrays, so c needs to be >=0 && <width().
rgb* image::operator [] (int r) const
{
  assert ( r >= 0 && r < height() );
  
  return raster[r];
}

//precondition: r >=0 and < height()  - check that in the function
//postcondition: returns that row of rgb's
//NOTE: usually you'd use this like so, image1[r][c];  however, there is no checking for out of bounds in built-in c++ arrays, so c needs to be >=0 && <width().
rgb* image::operator [] (int r)
{
  assert ( r >= 0 && r < height() );
  
  return raster[r];
}

*/
