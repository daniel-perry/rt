#include <cstring>

#include "RayTracer.h"

//#include "sphere_scene.h" 
//#include "volume_scene.h"
//#include "volume_scene_two_dist_mip.h"
//#include "volume_scene_tooth_mip.h"
//#include "volume_scene_two_dist.h"
//#include "volume_scene_tooth.h"
//#include "volume_scene_two_dist_curv.h"
//#include "volume_scene_tooth_curv.h"
//#include "volume_scene_curv.h"
#include "point_scene.h"

RayTracer::RayTracer(int width, int height)
:m_scene(0),
m_width(width),
m_height(height),
m_rc(),
m_buffer(0),
m_numComponents(4), // default r,g,b,a
m_state(RayTracer::DONE),
m_doneRendering(0),
m_numThreads(16),
m_threads(0),
m_regions(0)
{}

RayTracer::~RayTracer()
{
  // cleanup
  if(m_scene) delete m_scene;
  if(m_buffer) delete[] m_buffer;
}

//void RayTracer::makeScene(const std::string & nrrd_fn, const std::string & cmap_fn, float curvThick, bool normFlipped )
void RayTracer::makeScene(const std::string & points_fn, const std::string & values_fn, const std::string & cmap_fn )
{
  //m_scene = make_scene(nrrd_fn, cmap_fn, curvThick, normFlipped);
  m_scene = make_scene(points_fn, values_fn, cmap_fn);
  m_rc.scene = m_scene;
  m_scene->m_rc = m_rc;
  setSize(m_width,m_height);
}

void RayTracer::setSize(int width, int height)
{ 
  m_width = width;
  m_height = height;
  m_scene->imageX = width;
  m_scene->imageY = height;
  if(m_buffer) delete[] m_buffer;
  m_buffer = new unsigned char[ m_scene->imageX * m_scene->imageY * m_numComponents ](); // initialize w zero
}

void * thread_render(void * p)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // make this thread cancelable.

  Region * region = reinterpret_cast<Region*>(p);
  if(region)
  {
    region->rt->render(region->r0,region->c0,region->r1,region->c1);
  }
  return 0;
}


void RayTracer::restartRender()
{
  std::cerr << "rendering.." << std::endl;
  std::cerr << "camera: " << m_scene->camera->toString() << std::endl;

  m_state = RayTracer::RENDER;

  if(m_threads != 0)
  {
    // if we are restarting during a currently running
    // render, cancel the threads, before starting new
    // threads.
		/*
    for(size_t i=0; i<m_numThreads; ++i)
    {
      int error = pthread_cancel(m_threads[i]);
      if(error != 0)
      {
        std::cerr << "error canceling thread: " << error << std::endl;
      }
    }
		*/
		for(size_t i=0; i<m_numThreads; ++i)
    {
			void * result;
      int error = pthread_join(m_threads[i],&result);
      if(error != 0)
      {
        std::cerr << "error joining thread: " << error << std::endl;
      }
    }
    delete [] m_threads;
    delete [] m_regions;
  }

  m_threads = new pthread_t[m_numThreads];
  m_regions = new Region[m_numThreads];
  
  // reset buffer:
  memset(m_buffer, 0, m_numComponents*getWidth()*getHeight()*sizeof(buffer_type));

  /* Start up threads */
  pthread_attr_t pthread_custom_attr;
  pthread_attr_init(&pthread_custom_attr);
	
#if 1
  for (size_t i=0; i<m_numThreads; ++i)
  {
    m_regions[i].rt = this;
    m_regions[i].c0 = 0;
    m_regions[i].c1 = getWidth()-1;
    m_regions[i].r0 = i*(getHeight()/m_numThreads);
    m_regions[i].r1 = (i+1)*(getHeight()/m_numThreads)-1;
    m_regions[i].done = false;
    if( i == m_numThreads-1 )
    {
      m_regions[i].r1 = getHeight()-1; // make sure all rows covered.
    } 
    pthread_create(&m_threads[i], 
                   &pthread_custom_attr, 
                   thread_render, 
                   reinterpret_cast<void *>(&(m_regions[i])));
  }
#else
	//int num_rays = getHeight()*getWidth()*4;
	int num_rays = std::ceil(std::sqrt(getHeight()*getWidth()));
	float rand_max = RAND_MAX;
	for(int rays=0; rays<num_rays; ++rays)
	{
		for (size_t i=0; i<m_numThreads; ++i)
		{
			m_regions[i].rt = this;
			m_regions[i].c0 = std::floor(rand()/rand_max * getWidth());
			m_regions[i].c1 = m_regions[i].c0;
			m_regions[i].r0 = std::floor(rand()/rand_max * getHeight());
			m_regions[i].r1 = m_regions[i].r0;
			m_regions[i].done = false;
			if( i == m_numThreads-1 )
			{
				m_regions[i].r1 = getHeight()-1; // make sure all rows covered.
			} 
			pthread_create(&m_threads[i], 
										 &pthread_custom_attr, 
										 thread_render, 
										 reinterpret_cast<void *>(&(m_regions[i])));
		}
		for (size_t i=0; i<m_numThreads; ++i)
		{
			void * result;
      int error = pthread_join(m_threads[i],&result);
      if(error != 0)
      {
        std::cerr << "error joining thread: " << error << std::endl;
      }

		}
	}
	for(size_t i=0; i<m_numThreads; ++i) m_regions[i].done = true;
#endif

}

bool RayTracer::checkProgress()
{
  if( m_state == RayTracer::RENDER )
  {
    bool all_done = true;
    /* Synchronize the completion of each thread. */
    for (size_t i=0; i<m_numThreads; ++i)
    {
      if(!m_regions[i].done)
      {
        int retval = pthread_tryjoin_np(m_threads[i],NULL);
        if(retval == 0)
        {
          m_regions[i].done = true;
          //std::cerr << "retval: " << retval << std::endl;
          //std::cerr << "EDEADLK=" << EDEADLK << std::endl;
          //std::cerr << "EINVAL=" << EINVAL << std::endl;
          //std::cerr << "ESRCH=" << ESRCH << std::endl;
          //std::cerr << "EBUSY=" << EBUSY << std::endl;
          //std::cerr << "ETIMEDOUT=" << ETIMEDOUT << std::endl;
        }
        if(retval != 0)
        {
          all_done = false;
        }
      }
    }

    if(all_done)
    {
      delete [] m_threads;
      m_threads = 0;
      delete [] m_regions;
      m_regions = 0;
      m_state = RayTracer::DONE;
      (*m_doneRendering)(); // notify callback..
    }
  
    return all_done;
  }
  return true;
}

void RayTracer::render(size_t r0, size_t c0, size_t r1, size_t c1)
{
#if 0
  for(size_t i=r0; i<=r1; i++ ){
    for(size_t j=c0; j<=c1; j++){
      rgb clr;
      m_scene->sampler->sample( i , j , clr );
      set_color( i , j , clr);
    }
    pthread_testcancel();
  }
#elif 0
	static const float rand_max = RAND_MAX;
	for(size_t iter=0; iter < 2*(r1-r0)*(c1-c0); ++iter)
	{
		size_t i = std::floor((rand()/rand_max) * (r1-r0)) + r0;
		size_t j = std::floor((rand()/rand_max) * (c1-c0)) + c0;
		rgb clr;
		m_scene->sampler->sample( i , j , clr );
		set_color( i , j , clr);

		if(iter%(r1-r0) == 0) pthread_testcancel();
	}
#else
	static const float rand_max = RAND_MAX;
	int num_iters = 100000; //2*(r1-r0)*(c1-c0);
	for(int iter=0; iter < num_iters; ++iter)
	{
		size_t i = std::floor((rand()/rand_max) * getWidth());
		size_t j = std::floor((rand()/rand_max) * getHeight());
		rgb clr;
		get_color( i , j , clr ); // race condition ok, just want to skip if already colored.
		if(clr[0] == 0 && clr[1] == 0 && clr[2] == 0)
		{
			m_scene->sampler->sample( i , j , clr );
			set_color( i , j , clr);
		}

		if(iter%(r1-r0) == 0) pthread_testcancel();
	}
	// fill in any holes
	for(size_t i=r0; i<=r1; i++ ){
    for(size_t j=c0; j<=c1; j++){
      rgb clr;
			get_color( i , j , clr ); // race condition ok, just want to skip if already colored.
			if(clr[0] == 0 && clr[1] == 0 && clr[2] == 0)
			{
				m_scene->sampler->sample( i , j , clr );
				set_color( i , j , clr);
			}
    }
    pthread_testcancel();
  }
#endif

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
void RayTracer::get_color(size_t r, size_t c, Color &clr)
{
  size_t numComponents = 4;
  size_t width = m_scene->imageX;
  size_t height = m_scene->imageY;
  for(size_t i=0; i<numComponents-1; ++i)
  {
    clr[i] = static_cast<float>(m_buffer[ r*height*numComponents + c*numComponents + i ])/255; 
  }
}
