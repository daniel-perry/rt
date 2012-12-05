#include "RayTracer.h"

#include "req.h" 
//#include "volume_scene.h"

RayTracer::RayTracer(int width, int height)
:m_scene(0),
m_buffer(0),
m_numComponents(4), // default r,g,b,a
m_rc()
{
  m_scene = make_scene();
  m_rc.scene = m_scene;
  m_scene->rc = m_rc;

  setSize(width,height);
}

RayTracer::~RayTracer()
{
  // cleanup
  if(m_scene) delete m_scene;
  if(m_buffer) delete[] m_buffer;
}

void RayTracer::setSize(int width, int height)
{ 
  m_scene->imageX = width;
  m_scene->imageY = height;
  if(m_buffer) delete[] m_buffer;
  m_buffer = new unsigned char[ m_scene->imageX * m_scene->imageY * m_numComponents ](); // initialize w zero
}

void RayTracer::restartRender()
{
  std::cerr << "rendering.." << std::endl;
  std::cerr << "camera: " << m_scene->camera->toString() << std::endl;
  // TODO: spread this across a few threads..
  this->render(0,0,m_scene->imageX-1,m_scene->imageX-1);
  std::cerr << "done rendering." << std::endl;
  (*m_doneRendering)(); // notify callback..
}

void RayTracer::render(size_t r0, size_t c0, size_t r1, size_t c1)
{
  for( int i=r0; i<=r1; i++ ){
    for(int j=c0; j<=c1; j++){
      rgb clr;
      m_scene->sampler->sample( i , j , clr );
      set_color( i , j , clr);
    }
  }
}

void RayTracer::set_color(size_t r, size_t c, const Color &clr)
{
  size_t numComponents = 4;
  size_t width = m_scene->imageX;
  size_t height = m_scene->imageY;
  for(size_t i=0; i<numComponents-1; ++i)
  {
    m_buffer[ r*height*numComponents + c*numComponents + i ] = static_cast<unsigned char>(255*clr[i]);
  }
  m_buffer[ r*height*numComponents + c*numComponents + (numComponents-1) ] = static_cast<unsigned char>(255);
}
