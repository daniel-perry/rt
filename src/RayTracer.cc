
#include "RayTracer.h"

#include "req.h" 
//#include "volume_scene.h"

RayTracer::RayTracer(int width, int height)
:m_scene(0),
m_rc(),
m_buffer(0),
m_numComponents(4), // default r,g,b,a
m_state(RayTracer::DONE),
m_doneRendering(0),
m_numThreads(2),
m_threads(0),
m_regions(0)
{
  m_scene = make_scene();
  m_rc.scene = m_scene;
  m_scene->m_rc = m_rc;

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

void * thread_render(void * p)
{
  Region * region = reinterpret_cast<Region*>(p);
  if(region)
  {
    region->rt->render(region->r0,region->c0,region->r1,region->c1);
  }
}


void RayTracer::restartRender()
{
  std::cerr << "rendering.." << std::endl;
  std::cerr << "camera: " << m_scene->camera->toString() << std::endl;
  m_state = RayTracer::RENDER;

  if(m_threads)
  {
    // if we are restarting during a currently running
    // render, cancel the threads, before starting new
    // threads.
    for(size_t i=0; i<m_numThreads; ++i)
    {
      pthread_cancel(m_threads[i]);
    }
  }

  if( m_last_numThreads != m_numThreads )
  {
    if(m_threads) delete [] m_threads;
    if(m_regions) delete [] m_regions;
    m_threads = new pthread_t[m_numThreads];
    m_regions = new Region[m_numThreads];
    m_last_numThreads = m_numThreads;
  }

  /* Start up threads */
  pthread_attr_t pthread_custom_attr;
  pthread_attr_init(&pthread_custom_attr);
  for (size_t i=0; i<m_numThreads; ++i)
  {
    m_regions[i].rt = this;
    m_regions[i].c0 = 0;
    m_regions[i].c1 = getWidth()-1;
    m_regions[i].r0 = i*(getHeight()/m_numThreads);
    m_regions[i].r1 = (i+1)*(getHeight()/m_numThreads)-1;
    if( i == m_numThreads-1 )
    {
      m_regions[i].r1 = getHeight()-1; // make sure all rows covered.
    } 
    pthread_create(&m_threads[i], 
                   &pthread_custom_attr, 
                   thread_render, 
                   reinterpret_cast<void *>(&(m_regions[i])));
  }

}

bool RayTracer::checkProgress()
{
  if( m_state == RayTracer::RENDER )
  {
    std::cerr << "checkProgress" << std::endl;
    bool all_done = true;
    /* Synchronize the completion of each thread. */
    for (size_t i=0; i<m_numThreads; ++i)
    {
      //pthread_join(threads[i],NULL);
      if(pthread_tryjoin_np(m_threads[i],NULL) != 0)
      {
        all_done = false;
      }
    }

    if(all_done)
    {
      delete [] m_threads;
      delete [] m_regions;
      std::cerr << "done rendering.*****************************************" << std::endl;
      m_state = RayTracer::DONE;
      (*m_doneRendering)(); // notify callback..
    }
  
    return all_done;
  }
  return true;
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
