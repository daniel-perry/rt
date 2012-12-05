#include "RayTracer.h"

#include "req.h" 
//#include "volume_scene.h"

RayTracer::RayTracer(int width, int height)
:m_scene(0),
m_buffer(0),
m_numCompnents(4) // default r,g,b,a
{
  m_scene = make_scene();
  setSize(width,height);
}

Raytracer::~RayTracer()
{
  // cleanup
  if(m_scene) delete scene;
  if(m_buffer) delete[] m_buffer;
}

void RayTracer::setSize(int width, int height)
{ 
  m_scene->imageX = width;
  m_scene->imageY = height;
  if(m_buffer) delete[] m_buffer;
  m_buffer = new unsigned char[ m_scene->getWidth() * m_scene->getHeight() * m_numComponents ];
}

void RayTracer::restartRender()
{
  // TODO: spread this across a few threads..
  this->render(0,0,m_scene->getWidth()-1,m_scene->getHeight()-1);
}

void RayTracer::render(size_t r0, size_t c0, size_t r1, size_t c1);
{
  for( int i=r0; i<=r1; i++ ){
    for(int j=c0; j<=c1; j++){
      rgb clr;
      m_scene->sampler->sample( i , j , clr );
      set_color( i , j , clr);
    }
  }
}

void RayTracer::set_color(size_t r, size_t c, const Color &c)
{
  size_t numComponents = 4;
  for(size_t i=0; i<numComponents-1; ++i)
  {
    m_buffer[ r*height*numComponents + c*numComponents + i ] = static_cast<unsigned char>(255*c[i]);
  }
  m_buffer[ r*height*numComponents + c*numComponents + (numComponents-1) ] = static_cast<unsigned char>(255);
}
